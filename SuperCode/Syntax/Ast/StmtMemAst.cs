namespace SuperCode
{
	public class StmtMemAst: MemAst
	{
		public readonly StmtAst stmt;

		public StmtMemAst(StmtAst stmt)
		{
			token = stmt.token;
			this.stmt = stmt;
		}

		public override AstKind kind => AstKind.StmtMem;
	}
}
