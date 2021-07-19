namespace SuperCode
{
	public class TypePunExprAst: ExprAst
	{
		public Token open;
		public TypeAst type;
		public Token close;

		public ExprAst expr;

		public TypePunExprAst(Token open, TypeAst type, Token close, ExprAst expr)
		{
			this.open = open;
			this.type = type;
			this.close = close;
			this.expr = expr;
		}

		public override AstKind kind => AstKind.TypePunExpr;
	}
}
