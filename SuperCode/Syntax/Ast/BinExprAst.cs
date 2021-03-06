﻿namespace SuperCode
{
	public class BinExprAst: ExprAst
	{
		public readonly Token op;
		public readonly ExprAst left, right;

		public BinExprAst(Token op, ExprAst lhs, ExprAst rhs)
		{
			token = op;

			this.op = op;
			left = lhs;
			right = rhs;
		}

		public override AstKind kind => AstKind.BinExpr;
	}
}
