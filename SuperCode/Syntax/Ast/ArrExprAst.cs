namespace SuperCode
{
	public class ArrExprAst: ExprAst
	{
		public readonly Token open;
		public readonly ElementAst[] elements;
		public readonly Token close;

		public ArrExprAst(Token open, ElementAst[] elements, Token close)
		{
			this.open = open;
			this.elements = elements;
			this.close = close;
		}

		public override AstKind kind => AstKind.ArrExpr;
	}
}
