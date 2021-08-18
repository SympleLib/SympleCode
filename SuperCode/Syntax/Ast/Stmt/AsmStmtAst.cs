namespace SuperCode
{
	public class AsmStmtAst: StmtAst
	{
		public readonly Token key;
		public readonly Token asm;

		public AsmStmtAst(Token key, Token asm, Token semi): base(semi)
		{
			this.key = key;
			this.asm = asm;
		}

		public override AstKind kind => AstKind.AsmStmt;
	}
}
