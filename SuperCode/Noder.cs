using System;
using System.Collections.Generic;
using System.Xml.Linq;

using LLVMSharp.Interop;

namespace SuperCode
{
	public class Noder
	{
		public readonly ModuleAst module;

		private readonly Dictionary<string, Symbol> syms = new ();
		private LLVMTypeRef retType;

		public Noder(ModuleAst module) =>
			this.module = module;

		public ModuleNode Nodify()
		{
			var mems = new List<MemNode>();
			foreach (var mem in module.mems)
				mems.Add(Nodify(mem));
			return new ModuleNode(module.filename, mems.ToArray());
		}

		private ParamNode Nodify(ParamAst param) =>
			new (param.type.builtinType, param.name.text);

		private MemNode Nodify(MemAst mem)
		{
			switch (mem.kind)
			{
			case AstKind.FuncMem:
				return Nodify((FuncMemAst) mem);
			case AstKind.DeclFuncMem:
				return Nodify((DeclFuncMemAst) mem);

			default:
				throw new InvalidOperationException("Invalid mem");
			}
		}

		private FuncMemNode Nodify(FuncMemAst mem)
		{
			var paramTypes = new LLVMTypeRef[mem.paramz.Length];
			for (int i = 0; i < paramTypes.Length; i++)
				paramTypes[i] = mem.paramz[i].type.builtinType;

			var paramz = new List<ParamNode>();
			foreach (var param in mem.paramz)
			{
				var paramNode = Nodify(param);
				paramz.Add(paramNode);
				syms.Add(paramNode.name, paramNode);
			}

			var ty = LLVMTypeRef.CreateFunction(mem.retType.builtinType, paramTypes);
			retType = ty.ReturnType;
			string name = mem.name.text;


			var stmts = new List<StmtNode>();
			foreach (var stmt in mem.stmts)
				stmts.Add(Nodify(stmt));

			var func = new FuncMemNode(ty, name, paramz.ToArray(), stmts.ToArray());
			syms.Add(name, func);
			return func;
		}

		private DeclFuncMemNode Nodify(DeclFuncMemAst mem)
		{
			var paramTypes = new LLVMTypeRef[mem.paramz.Length];
			for (int i = 0; i < paramTypes.Length; i++)
				paramTypes[i] = mem.paramz[i].type.builtinType;

			var paramz = new List<ParamNode>();
			foreach (var param in mem.paramz)
			{
				var paramNode = Nodify(param);
				paramz.Add(paramNode);
				if (paramNode.name is not null)
					syms.Add(paramNode.name, paramNode);
			}

			var ty = LLVMTypeRef.CreateFunction(mem.retType.builtinType, paramTypes);
			string name = mem.name.text;

			var decl = new DeclFuncMemNode(ty, name, paramz.ToArray());
			syms.Add(name, decl);
			return decl;
		}

		private StmtNode Nodify(StmtAst stmt)
		{
			switch (stmt.kind)
			{
			case AstKind.RetStmt:
				return Nodify((RetStmtAst) stmt);
			case AstKind.VarStmt:
				return Nodify((VarStmtAst) stmt);
			case AstKind.ExprStmt:
				return Nodify(((ExprStmtAst) stmt).expr);

			default:
				throw new InvalidOperationException("Invalid stmt");
			}
		}

		private RetStmtNode Nodify(RetStmtAst stmt) =>
			new (Nodify(stmt.value, retType));

		private VarStmtNode Nodify(VarStmtAst stmt)
		{
			var ty = stmt.type.builtinType;
			var var = new VarStmtNode(ty, stmt.name.text, Nodify(stmt.init, ty));
			syms.Add(var.name, var);
			return var;
		}

		private ExprNode Nodify(ExprAst expr, LLVMTypeRef castTo = default)
		{
			switch (expr.kind)
			{
			case AstKind.LitExpr:
				return Cast(Nodify((LitExprAst) expr), castTo);
			case AstKind.BinExpr:
				return Cast(Nodify((BinExprAst) expr), castTo);
			case AstKind.CallExpr:
				return Cast(Nodify((CallExprAst) expr), castTo);
			case AstKind.CastExpr:
				return Cast(Nodify((CastExprAst) expr), castTo);
			case AstKind.ParenExpr:
				return Cast(Nodify(((ParenExprAst) expr).expr), castTo);

			default:
				throw new InvalidOperationException("Invalid expr");
			}
		}

		private ExprNode Nodify(LitExprAst expr)
		{
			string literal = expr.literal.text;
			switch (expr.literal.kind)
			{
			case TokenKind.Iden:
				return new SymExprNode(syms[literal]);
			case TokenKind.Num:
				if (literal.Contains('.'))
				{
					double num = double.Parse(literal);
					var ty = num != (float) num ? LLVMTypeRef.Double : LLVMTypeRef.Float;
					return new FNumExprNode(num, ty);
				}
				else
				{
					ulong num = ulong.Parse(literal);
					var ty = num != (uint) num ? LLVMTypeRef.Int64 : LLVMTypeRef.Int32;
					return new NumExprNode(num, ty);
				}

			default:
				throw new InvalidOperationException("Invalid lit-expr");
			}
		}

		private BinExprNode Nodify(BinExprAst expr)
		{
			var left = Nodify(expr.left);

			BinOp op;
			switch (expr.op.kind)
			{
			case TokenKind.Plus:
				op = BinOp.Add;
				goto BinExpr;
			case TokenKind.Dash:
				op = BinOp.Sub;
				goto BinExpr;
			case TokenKind.Star:
				op = BinOp.Mul;
				goto BinExpr;
			case TokenKind.Slash:
				op = BinOp.Div;
				goto BinExpr;
			case TokenKind.Percent:
				op = BinOp.Mod;
				goto BinExpr;

			default:
				throw new InvalidOperationException("Invalid lit-expr");
			}

		BinExpr:
			var right = Nodify(expr.right, left.type);
			return new BinExprNode(op, left, right);
		}

		private CallExprNode Nodify(CallExprAst expr)
		{
			var what = Nodify(expr.what);
			var args = new List<ExprNode>();
			for (int i = 0; i < expr.args.Length; i++)
				args.Add(Nodify(expr.args[i], what.type.ParamTypes[i]));

			return new CallExprNode(what, args.ToArray());
		}

		private ExprNode Nodify(CastExprAst expr) =>
			Nodify(expr.value, expr.type.builtinType);

		private ExprNode Cast(ExprNode node, LLVMTypeRef to)
		{
			if (to == default || node.type == to)
				return node;
			return new CastExprNode(node, to);
		}
	}
}
