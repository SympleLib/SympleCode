﻿namespace SuperCode
{
	public class ExprStmtAst: StmtAst
	{
		public readonly ExprAst expr;

		public ExprStmtAst(ExprAst expr) =>
			this.expr = expr;

		public override AstKind kind => AstKind.ExprStmt;
	}
}
