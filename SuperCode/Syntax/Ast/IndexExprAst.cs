namespace SuperCode
{
	public class IndexExprAst: ExprAst
	{
		public readonly ExprAst expr;
		public readonly Token open;
		public readonly ExprAst index;
		public readonly Token close;

		public IndexExprAst(ExprAst expr, Token open, ExprAst index, Token close)
		{
			this.expr = expr;
			this.open = open;
			this.index = index;
			this.close = close;
		}

		public override AstKind kind => AstKind.IndexExpr;
	}
}
