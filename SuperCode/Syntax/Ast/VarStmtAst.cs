namespace SuperCode
{
	public class VarStmtAst: StmtAst
	{
		public readonly Token key;
		public readonly Token name;
		public readonly Token eql;
		public readonly ExprAst init;

		public VarStmtAst(Token key, Token name, Token eql, ExprAst init, Token semi): base(semi)
		{
			this.key = key;
			this.name = name;
			this.eql = eql;
			this.init = init;
		}

		public override AstKind kind => AstKind.VarStmt;
	}
}
