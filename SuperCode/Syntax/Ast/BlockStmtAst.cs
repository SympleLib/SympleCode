namespace SuperCode
{
	public class BlockStmtAst: StmtAst
	{
		public readonly Token open;
		public readonly StmtAst[] stmts;
		public readonly Token close;

		public BlockStmtAst(Token open, StmtAst[] stmts, Token close)
		{
			token = open;

			this.open = open;
			this.stmts = stmts;
			this.close = close;
		}

		public override AstKind kind => AstKind.BlockStmt;
	}
}
