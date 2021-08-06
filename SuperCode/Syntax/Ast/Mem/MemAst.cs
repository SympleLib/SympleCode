namespace SuperCode
{
	public abstract class MemAst: Ast
	{
		public readonly Token? semicol;

		public MemAst(Token? semi = null) =>
			semicol = semi;
	}
}
