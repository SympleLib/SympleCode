namespace SuperCode
{
	public class UsingStmtAst: StmtAst
	{
		public readonly Token key;
		public readonly TypeAst real;
		public readonly Token asKey;
		public readonly Token alias;

		public UsingStmtAst(Token key, TypeAst real, Token asKey, Token alias, Token semi): base(semi)
		{
			token = alias;

			this.key = key;
			this.real = real;
			this.asKey = asKey;
			this.alias = alias;
		}

		public override AstKind kind => AstKind.UsingStmt;
	}
}
