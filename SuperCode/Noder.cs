using System;
using System.Collections.Generic;

using LLVMSharp.Interop;

namespace SuperCode
{
	public class Noder
	{
		public readonly ModuleAst module;

		private readonly Dictionary<string, VarStmtNode> vars = new ();

		public Noder(ModuleAst module) =>
			this.module = module;

		public ModuleNode Nodify()
		{
			var mems = new List<MemNode>();
			foreach (var mem in module.mems)
				mems.Add(Nodify(mem));
			return new ModuleNode(module.filename, mems.ToArray());
		}

		private MemNode Nodify(MemAst mem)
		{
			switch (mem.kind)
			{
			case AstKind.FuncMem:
				return Nodify((FuncMemAst) mem);

			default:
				throw new InvalidOperationException("Invalid mem");
			}
		}

		private FuncMemNode Nodify(FuncMemAst mem)
		{
			var ty = mem.retType.builtinType;
			string name = mem.name.text;
			var stmts = new List<StmtNode>();
			foreach (var stmt in mem.stmts)
				stmts.Add(Nodify(stmt));

			return new FuncMemNode(ty, name, stmts.ToArray());
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
			new (Nodify(stmt.value));

		private VarStmtNode Nodify(VarStmtAst stmt)
		{
			var ty = stmt.type.builtinType;
			var var = new VarStmtNode(ty, stmt.name.text, Nodify(stmt.init));
			vars.Add(var.name, var);
			return var;
		}

		private ExprNode Nodify(ExprAst expr)
		{
			switch (expr.kind)
			{
			case AstKind.LitExpr:
				return Nodify((LitExprAst) expr);
			case AstKind.BinExpr:
				return Nodify((BinExprAst) expr);

			default:
				throw new InvalidOperationException("Invalid expr");
			}
		}

		private ExprNode Nodify(LitExprAst expr)
		{
			switch (expr.literal.kind)
			{
			case TokenKind.Iden:
				return new VarExprNode(vars[expr.literal.text]);
			case TokenKind.Num:
				if (expr.literal.text.Contains('.'))
				{
					double num = double.Parse(expr.literal.text);
					var ty = num != (float) num ? LLVMTypeRef.Double : LLVMTypeRef.Float;
					return new FNumExprNode(num, ty);
				}
				else
				{
					ulong num = ulong.Parse(expr.literal.text);
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
			var right = Nodify(expr.right);
			return new BinExprNode(op, left, right);
		}
	}
}
