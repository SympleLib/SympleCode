namespace SuperCode
{
	public class VarStmtAst: StmtAst
	{
		public readonly TypeAst type;
		public readonly Token name;
		public readonly Token eql;
		public readonly ExprAst init;

		public VarStmtAst(TypeAst ty, Token name, Token eql, ExprAst init, Token semi): base(semi)
		{
			token = eql == default ? name : eql;

			this.type = ty;
			this.name = name;
			this.eql = eql;
			this.init = init;
		}

		public override AstKind kind => AstKind.VarStmt;
	}
}
