namespace SuperCode
{
	public class CastExprAst: ExprAst
	{
		public readonly Token open;
		public readonly TypeAst type;
		public readonly Token close;
		public readonly ExprAst expr;

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
