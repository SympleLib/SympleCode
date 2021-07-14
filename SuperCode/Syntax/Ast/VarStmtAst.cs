namespace SuperCode
{
	public class VarStmtAst: StmtAst
	{
		public readonly Token name;
		public readonly Token eql;
		public readonly ExprAst init;

		public VarStmtAst(Token name, Token eql, ExprAst init)
		{
			this.name = name;
			this.eql = eql;
			this.init = init;
		}

		public override AstKind kind => AstKind.VarStmt;
	}
}
