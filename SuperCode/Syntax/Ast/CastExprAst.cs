namespace SuperCode
{
	public class CastExprAst: ExprAst
	{
		public Token open;
		public TypeAst type;
		public Token close;
		public ExprAst expr;

		public CastExprAst(Token open, TypeAst type, Token close, ExprAst expr)
		{
			token = open;

			this.open = open;
			this.type = type;
			this.close = close;
			this.expr = expr;
		}

		public override AstKind kind => AstKind.CastExpr;
	}
}
