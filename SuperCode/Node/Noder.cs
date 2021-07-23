using System;
using System.Collections.Generic;
using System.Xml.Linq;

using LLVMSharp.Interop;

using static LLVMSharp.AtomicRMWInst;

namespace SuperCode
{
	public class Noder
	{
		public readonly PermaSafe safety = new ();
		public readonly ModuleAst module;

		private readonly Dictionary<string, Symbol> syms = new ();
		private TypeNode retType;

		public Noder(ModuleAst module) =>
			this.module = module;

		public PermaSafe Nodify(out ModuleNode node)
		{
			var mems = new List<MemNode>();
			foreach (var mem in module.mems)
				mems.Add(Nodify(mem));
			node = new (module.filename, mems.ToArray()) { syntax = module };
			return safety;
		}

		private TypeNode Nodify(TypeAst ast)
		{
			var ty = ast.baze.builtinType;
			for (int i = 0; i < ast.addons.Length; i++)
				ty = ty.Ref();
			return ty;
		}

		private ParamNode Nodify(ParamAst ast) =>
			new (Nodify(ast.type), ast.name.text) { syntax = ast };

		private MemNode Nodify(MemAst ast)
		{
			switch (ast.kind)
			{
			case AstKind.FuncMem:
				return Nodify((FuncMemAst) ast);
			case AstKind.DeclFuncMem:
				return Nodify((DeclFuncMemAst) ast);

			default:
				throw new InvalidOperationException("Invalid mem");
			}
		}

		private FuncMemNode Nodify(FuncMemAst ast)
		{
			var paramTypes = new TypeNode[ast.paramz.Length];
			var paramLLTypes = new LLVMTypeRef[ast.paramz.Length];
			for (int i = 0; i < paramTypes.Length; i++)
				paramLLTypes[i] = (paramTypes[i] = Nodify(ast.paramz[i].type)).lltype;

			var paramz = new List<ParamNode>();
			foreach (var param in ast.paramz)
			{
				var paramNode = Nodify(param);
				paramz.Add(paramNode);
				syms.Add(paramNode.name, paramNode);
			}

			retType = Nodify(ast.retType);
			var ty = LLVMTypeRef.CreateFunction(retType.lltype, paramLLTypes);
			string name = ast.name.text;


			var stmts = new List<StmtNode>();
			foreach (var stmt in ast.stmts)
				stmts.Add(Nodify(stmt));

			var func = new FuncMemNode(new TypeNode(ty), name, paramz.ToArray(), stmts.ToArray()) { syntax = ast };
			syms.Add(name, func);
			return func;
		}

		private DeclFuncMemNode Nodify(DeclFuncMemAst ast)
		{
			var paramTypes = new TypeNode[ast.paramz.Length];
			var paramLLTypes = new LLVMTypeRef[ast.paramz.Length];
			for (int i = 0; i < paramTypes.Length; i++)
				paramLLTypes[i] = (paramTypes[i] = Nodify(ast.paramz[i].type)).lltype;

			var paramz = new List<ParamNode>();
			foreach (var param in ast.paramz)
			{
				var paramNode = Nodify(param);
				paramz.Add(paramNode);
				if (paramNode.name is not null)
					syms.Add(paramNode.name, paramNode);
			}

			retType = Nodify(ast.retType);
			var ty = LLVMTypeRef.CreateFunction(retType.lltype, paramLLTypes);
			string name = ast.name.text;

			var decl = new DeclFuncMemNode(new TypeNode(ty), name, paramz.ToArray()) { syntax = ast };
			syms.Add(name, decl);
			return decl;
		}

		private StmtNode Nodify(StmtAst ast)
		{
			switch (ast.kind)
			{
			case AstKind.RetStmt:
				return Nodify((RetStmtAst) ast);
			case AstKind.VarStmt:
				return Nodify((VarStmtAst) ast);
			case AstKind.ExprStmt:
				return Nodify(((ExprStmtAst) ast).expr);

			default:
				throw new InvalidOperationException("Invalid stmt");
			}
		}

		private RetStmtNode Nodify(RetStmtAst ast) =>
			new (Nodify(ast.value, retType));

		private VarStmtNode Nodify(VarStmtAst ast)
		{
			var ty = Nodify(ast.type);
			var var = new VarStmtNode(ty, ast.name.text, Nodify(ast.init, ty)) { syntax = ast };
			syms.Add(var.name, var);
			return var;
		}

		private ExprNode Nodify(ExprAst ast, TypeNode castTo = default)
		{
			switch (ast.kind)
			{
			case AstKind.LitExpr:
				return Cast(Nodify((LitExprAst) ast), castTo);
			case AstKind.BinExpr:
				return Cast(Nodify((BinExprAst) ast), castTo);
			case AstKind.CallExpr:
				return Cast(Nodify((CallExprAst) ast), castTo);
			case AstKind.CastExpr:
				return Cast(Nodify((CastExprAst) ast), castTo);
			case AstKind.ParenExpr:
				return Cast(Nodify(((ParenExprAst) ast).expr), castTo);
			case AstKind.PreExpr:
				return Cast(Nodify((PreExprAst) ast), castTo);
			case AstKind.TypePunExpr:
				return Cast(Nodify((TypePunExprAst) ast), castTo);

			default:
				throw new InvalidOperationException("Invalid expr");
			}
		}

		private ExprNode Nodify(LitExprAst ast)
		{
			string literal = ast.literal.text;
			switch (ast.literal.kind)
			{
			case TokenKind.Iden:
				return new SymExprNode(syms[literal]) { syntax = ast };
			case TokenKind.Str:
				return new StrExprNode(literal[1..^1]) { syntax = ast };
			case TokenKind.Num:
				if (literal.Contains('.'))
				{
					bool isF32 = literal.Contains('f') || literal.Contains('F');
					double num = double.Parse(isF32 ? literal[..^1] : literal);
					var ty = isF32 ? TypeNode.fp32 : TypeNode.fp64;
					return new FNumExprNode(num, ty) { syntax = ast };
				}
				else
				{
					ulong num = ulong.Parse(literal);
					var ty = num != (uint) num ? TypeNode.i64 : TypeNode.i32;
					return new NumExprNode(num, ty) { syntax = ast };
				}

			default:
				throw new InvalidOperationException("Invalid lit-expr");
			}
		}

		private BinExprNode Nodify(BinExprAst ast)
		{
			var left = Nodify(ast.left);
			var right = Nodify(ast.right, left.type);
			bool fp = left.type.isFloat;
			bool us = left.type.unsigned;
			
			BinOp op;
			switch (ast.op.kind)
			{
			case TokenKind.Plus:
				op = fp ? BinOp.FAdd : BinOp.Add;
				goto BinExpr;
			case TokenKind.Dash:
				op = fp ? BinOp.FSub : BinOp.Sub;
				goto BinExpr;
			case TokenKind.Star:
				op = fp ? BinOp.FMul : BinOp.Mul;
				goto BinExpr;
			case TokenKind.Slash:
				op = fp ? BinOp.FDiv : us ? BinOp.UDiv : BinOp.SDiv;
				goto BinExpr;
			case TokenKind.Percent:
				op = fp ? BinOp.FMod : us ? BinOp.UMod : BinOp.SMod;
				goto BinExpr;

			default:
				throw new InvalidOperationException("Invalid lit-expr");
			}

		BinExpr:
			return new BinExprNode(op, left, right) { syntax = ast };
		}

		private CallExprNode Nodify(CallExprAst ast)
		{
			var what = Nodify(ast.what);
			var args = new List<ExprNode>();
			for (int i = 0; i < ast.args.Length; i++)
				args.Add(Nodify(ast.args[i], new TypeNode(what.type.lltype.ParamTypes[i])));

			return new CallExprNode(what, args.ToArray()) { syntax = ast };
		}

		private ExprNode Nodify(CastExprAst ast)
		{
			var node = Nodify(ast);
			var to = Nodify(ast.type);
			if (to == default || node.type == to)
				return node;
			return new CastExprNode(node, to) { syntax = ast };
		}

		private ExprNode Nodify(PreExprAst ast)
		{
			if (ast.prefix.Is(TokenKind.Plus))
				return Nodify(ast.expr);

			UnOp op;
			var var = Nodify(ast.expr);
			var ty = var.type;
			bool fp = ty.isFloat;

			switch (ast.prefix.kind)
			{
			case TokenKind.Dash:
				op = fp ? UnOp.FNeg : UnOp.Neg;
				goto UnExpr;
			case TokenKind.At:
				op = UnOp.Ref;
				ty.Ref();
				goto UnExpr;
			case TokenKind.Percent:
				op = UnOp.Deref;
				ty = ty.Deref();
				goto UnExpr;

			default:
				throw new InvalidOperationException("Invalid un-expr");
			}

		UnExpr:
			return new UnExprNode(ty, op, var) { syntax = ast };
		}

		private ExprNode Nodify(TypePunExprAst ast)
		{
			var ty = Nodify(ast.type);
			var expr = Nodify(ast.expr);
			if (ty == expr.type)
				return expr;
			return new TypePunExprNode(ty, expr) { syntax = ast };
		}

		private ExprNode Cast(ExprNode node, TypeNode to)
		{
			if (to == default || node.type == to)
				return node;

			if (to.lltype.IntWidth < node.type.lltype.IntWidth && !to.isFloat && !to.isPtr)
				safety.ReportPossibleLossOfData(node.syntax.token);
			return new CastExprNode(node, to);
		}
	}
}
