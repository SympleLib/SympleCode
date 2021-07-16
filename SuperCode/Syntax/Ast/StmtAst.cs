namespace SuperCode
{
	public abstract class StmtAst: Ast
	{
		public readonly Token semicol;

		public StmtAst() {}
		public StmtAst(Token semi) =>
			semicol = semi;
	}
}
