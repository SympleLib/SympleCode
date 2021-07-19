namespace SuperCode
{
	public class PreExprAst: ExprAst
	{
		public readonly Token prefix;
		public readonly ExprAst expr;

		public PreExprAst(Token prefix, ExprAst expr)
		{
			this.prefix = prefix;
			this.expr = expr;
		}

		public override AstKind kind => AstKind.PreExpr;
	}
}
