﻿using System;
using System.Collections.Generic;

namespace SuperCode
{
	public class Noder
	{
		public readonly ModuleAst module;

		public Noder(ModuleAst module) =>
			this.module = module;

		public ModuleNode Nodify()
		{
			var stmts = new List<StmtNode>();
			foreach (var stmt in module.stmts)
				stmts.Add(Nodify(stmt));
			return new ModuleNode(stmts.ToArray());
		}

		private StmtNode Nodify(StmtAst stmt)
		{
			switch (stmt.kind)
			{
			case AstKind.VarStmt:
				return Nodify((VarStmtAst) stmt);
			case AstKind.ExprStmt:
				return Nodify(((ExprStmtAst) stmt).expr);

			default:
				throw new InvalidOperationException("Invalid stmt");
			}
		}

		private VarStmtNode Nodify(VarStmtAst stmt) =>
			new (stmt.name.text, Nodify(stmt.init));

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
				return new VarExprNode(expr.literal.text);
			case TokenKind.Num:
				return new NumExprNode(ulong.Parse(expr.literal.text));

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
