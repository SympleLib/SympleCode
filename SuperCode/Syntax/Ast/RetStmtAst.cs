namespace SuperCode
{
	public class RetStmtAst: StmtAst
	{
		public readonly Token key;
		public readonly ExprAst value;

		public RetStmtAst(Token key, ExprAst val, Token semi): base(semi)
		{
			token = key;

			this.key = key;
			value = val;
		}

		public override AstKind kind => AstKind.RetStmt;
	}
}
