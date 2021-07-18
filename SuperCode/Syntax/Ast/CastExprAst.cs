namespace SuperCode
{
	public class CastExprAst: ExprAst
	{
		public Token open;
		public TypeAst type;
		public Token close;
		public ExprAst value;

		public CastExprAst(Token open, TypeAst type, Token close, ExprAst value)
		{
			this.open = open;
			this.type = type;
			this.close = close;
			this.value = value;
		}

		public override AstKind kind => AstKind.CastExpr;
	}
}
