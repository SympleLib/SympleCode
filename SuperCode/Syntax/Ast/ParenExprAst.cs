namespace SuperCode
{
	public class ParenExprAst: ExprAst
	{
		public Token open;
		public ExprAst expr;
		public Token close;

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
