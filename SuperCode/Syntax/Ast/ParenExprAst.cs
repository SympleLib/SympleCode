namespace SuperCode
{
	public class ParenExprAst: ExprAst
	{
		public readonly Token open;
		public readonly ExprAst expr;
		public readonly Token close;

		public ParenExprAst(Token open, ExprAst expr, Token close)
		{
			token = open;

			this.open = open;
			this.expr = expr;
			this.close = close;
		}

		public override AstKind kind => AstKind.ParenExpr;
	}
}
