namespace SuperCode
{
	public class CallExprAst: ExprAst
	{
		public readonly ExprAst what;

		public readonly Token open;
		public readonly ExprAst[] args;
		public readonly Token close;

		public CallExprAst(ExprAst what, Token open, ExprAst[] args, Token close)
		{
			this.what = what;
			this.open = open;
			this.args = args;
			this.close = close;
		}

		public override AstKind kind => AstKind.CallExpr;
	}
}
