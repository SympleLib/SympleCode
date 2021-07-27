using System;
using System.Collections.Generic;
using System.Xml.Linq;

using LLVMSharp.Interop;

namespace SuperCode
{
	public class Noder
	{
		public readonly PermaSafe safety = new ();
		public readonly ModuleAst module;
		public readonly LLVMContextRef ctx;

		private readonly Dictionary<string, Symbol> syms = new ();
		private readonly Dictionary<string, LLVMTypeRef> types = new ();
		private readonly Dictionary<LLVMTypeRef, StructMemNode> ztructs = new ();
		private LLVMTypeRef retType;

		public Noder(ModuleAst module)
		{
			this.module = module;
			ctx = LLVMContextRef.Global;
		}

		public PermaSafe Nodify(out ModuleNode node)
		{
			var mems = new MemNode[module.mems.Length];
			for (int i = 0; i < mems.Length; i++)
				mems[i] = Nodify(module.mems[i]);
			node = new ModuleNode(module.filename, mems) { syntax = module };
			return safety;
		}

		private LLVMTypeRef Nodify(TypeAst ast)
		{
			LLVMTypeRef ty;
			if (ast.baze.isBuiltinType)
				ty = ast.baze.builtinType;
			else
				ty = types[ast.baze.text];
			for (int i = 0; i < ast.addons.Length; i++)
				ty = ty.Ref();
			return ty;
		}

		private FieldNode Nodify(FieldAst ast) =>
			new (Nodify(ast.type), ast.name.text) { syntax = ast };

		private MemNode Nodify(MemAst ast)
		{
			switch (ast.kind)
			{
			case AstKind.DeclFuncMem:
				return Nodify((DeclFuncMemAst) ast);
			case AstKind.FuncMem:
				return Nodify((FuncMemAst) ast);
			case AstKind.StructMem:
				return Nodify((StructMemAst) ast);

			default:
				throw new InvalidOperationException("Invalid mem");
			}
		}

		private DeclFuncMemNode Nodify(DeclFuncMemAst ast)
		{
			var paramTypes = new LLVMTypeRef[ast.paramz.Length];
			for (int i = 0; i < paramTypes.Length; i++)
				paramTypes[i] = Nodify(ast.paramz[i].type);

			var paramz = new FieldNode[ast.paramz.Length];
			for (int i = 0; i < paramz.Length; i++)
			{
				var paramNode = Nodify(ast.paramz[i]);
				paramz[i] = paramNode;
				if (paramNode.name is not null)
					syms.Add(paramNode.name, paramNode);
			}

			retType = Nodify(ast.retType);
			var ty = LLVMTypeRef.CreateFunction(retType, paramTypes);
			string name = ast.name.text;

			var decl = new DeclFuncMemNode(ty, name, paramz) { syntax = ast };
			syms.Add(name, decl);
			return decl;
		}

		private FuncMemNode Nodify(FuncMemAst ast)
		{
			var paramTypes = new LLVMTypeRef[ast.paramz.Length];
			for (int i = 0; i < paramTypes.Length; i++)
				paramTypes[i] = Nodify(ast.paramz[i].type);

			var paramz = new FieldNode[ast.paramz.Length];
			for (int i = 0; i < paramz.Length; i++)
			{
				var paramNode = Nodify(ast.paramz[i]);
				paramz[i] = paramNode;
				syms.Add(paramNode.name, paramNode);
			}

			retType = Nodify(ast.retType);
			var ty = LLVMTypeRef.CreateFunction(retType, paramTypes);
			string name = ast.name.text;

			var stmts = new StmtNode[ast.stmts.Length];
			for (int i = 0; i < stmts.Length; i++)
				stmts[i] = Nodify(ast.stmts[i]);

			var func = new FuncMemNode(ty, name, paramz, stmts) { syntax = ast };
			syms.Add(name, func);
			return func;
		}

		private StructMemNode Nodify(StructMemAst ast)
		{
			var fields = new FieldNode[ast.fields.Length];
			var fieldTypes = new LLVMTypeRef[ast.fields.Length];
			for (int i = 0; i < fields.Length; i++)
			{
				var field = Nodify(ast.fields[i]);
				fields[i] = field;
				fieldTypes[i] = field.type;
			}

			string name = ast.name.text;
			var type = ctx.CreateNamedStruct(name);
			type.StructSetBody(fieldTypes, true);
			var node = new StructMemNode(type, name, fields);
			ztructs.Add(type, node);
			types.Add(name, type);
			return node;
		}


		private StmtNode Nodify(StmtAst ast)
		{
			switch (ast.kind)
			{
			case AstKind.ExprStmt:
				return Nodify(((ExprStmtAst) ast).expr);
			case AstKind.RetStmt:
				return Nodify((RetStmtAst) ast);
			case AstKind.VarStmt:
				return Nodify((VarStmtAst) ast);

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

		private ExprNode Nodify(ExprAst ast, LLVMTypeRef castTo = default)
		{
			if (ast is null)
				return null;

			switch (ast.kind)
			{
			case AstKind.BinExpr:
				return Cast(Nodify((BinExprAst) ast), castTo);
			case AstKind.CallExpr:
				return Cast(Nodify((CallExprAst) ast), castTo);
			case AstKind.CastExpr:
				return Cast(Nodify((CastExprAst) ast), castTo);
			case AstKind.LitExpr:
				return Cast(Nodify((LitExprAst) ast), castTo);
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

		private ExprNode Nodify(BinExprAst ast)
		{
			var left = Nodify(ast.left);
			switch (ast.op.kind)
			{
			case TokenKind.Dot:
			{
				string name = ast.right.token.text;
				return new MemExprNode(left, name, MemI(left.type, name), MemTy(left.type, name));
			}
			}

			var right = Nodify(ast.right, left.type);
			bool fp = left.type.IsFloat();
			
			BinOp op;
			switch (ast.op.kind)
			{
			case TokenKind.Eql:
				return new AssignExprNode(left, right);

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
				op = fp ? BinOp.FDiv : BinOp.SDiv;
				goto BinExpr;
			case TokenKind.Percent:
				op = fp ? BinOp.FMod : BinOp.SMod;
				goto BinExpr;
			case TokenKind.SlashDash:
				op = BinOp.UDiv;
				goto BinExpr;
			case TokenKind.PercentDash:
				op = BinOp.UMod;
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
			var args = new ExprNode[ast.args.Length];
			for (int i = 0; i < args.Length; i++)
				args[i] = Nodify(ast.args[i], what.type.ParamTypes[i]);

			return new CallExprNode(what, args) { syntax = ast };
		}

		private ExprNode Nodify(CastExprAst ast)
		{
			var node = Nodify(ast.expr);
			var to = Nodify(ast.type);
			if (to == default || node.type == to)
				return node;
			return new CastExprNode(node, to) { syntax = ast };
		}

		private ExprNode Nodify(LitExprAst ast)
		{
			string literal = ast.literal.text;
			switch (ast.literal.kind)
			{
			case TokenKind.Iden:
				return new SymExprNode(syms[literal]) { syntax = ast };
			case TokenKind.Str:
			{
				int start = literal[0] == '\'' ? 1 : 2;
				StrType type = literal[0] switch
				{
					's' => StrType.Short,
					'u' => StrType.Unicode,
					'w' => StrType.Wide,

					_ => StrType.Unicode,
				};

				return new StrExprNode(type, literal[start..^1]) { syntax = ast };
			}
			case TokenKind.Num:
				if (literal.Contains('.'))
				{
					bool isF32 = literal.Contains('f') || literal.Contains('F');
					double num = double.Parse(isF32 ? literal[..^1] : literal);
					var ty = isF32 ? LLVMTypeRef.Float : LLVMTypeRef.Double;
					return new FNumExprNode(num, ty) { syntax = ast };
				}
				else
				{
					ulong num = ulong.Parse(literal);
					var ty = num != (uint) num ? LLVMTypeRef.Int64 : LLVMTypeRef.Int32;
					return new NumExprNode(num, ty) { syntax = ast };
				}

			default:
				throw new InvalidOperationException("Invalid lit-expr");
			}
		}

		private ExprNode Nodify(PreExprAst ast)
		{
			if (ast.prefix.Is(TokenKind.Plus))
				return Nodify(ast.expr);

			UnOp op;
			var var = Nodify(ast.expr);
			var ty = var.type;
			bool fp = ty.IsFloat();

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
				ty = ty.ElementType;
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

		private ExprNode Cast(ExprNode node, LLVMTypeRef to)
		{
			if (to == default || node.type == to)
				return node;

			if (to.IntWidth < node.type.IntWidth && !to.IsFloat() && !to.IsPtr())
				safety.ReportPossibleLossOfData(node.syntax.token);
			return new CastExprNode(node, to);
		}

		private LLVMTypeRef MemTy(LLVMTypeRef ty, string name)
		{
			var ztruct = ztructs[ty];
			foreach (var field in ztruct.fields)
				if (field.name == name)
					return field.type;
			throw new InvalidOperationException("Member not found");
		}

		private int MemI(LLVMTypeRef ty, string name)
		{
			var ztruct = ztructs[ty];
			for (int i = 0; i < ztruct.fields.Length; i++)
				if (ztruct.fields[i].name == name)
					return i;
			throw new InvalidOperationException("Member not found");
		}
	}
}
