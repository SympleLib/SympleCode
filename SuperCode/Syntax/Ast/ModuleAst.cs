namespace SuperCode
{
	public class ModuleAst: Ast
	{
		public readonly StmtAst[] stmts;
		public readonly Token eof;

		public ModuleAst(StmtAst[] stmts, Token eof)
		{
			this.stmts = stmts;
			this.eof = eof;
		}

		public override AstKind kind => AstKind.Module;
	}
}
