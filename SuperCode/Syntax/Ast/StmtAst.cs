namespace SuperCode
{
	public abstract class StmtAst: Ast
	{
		public readonly Token? semicol;

		public StmtAst(Token? semi = null) =>
			semicol = semi;
	}
}
