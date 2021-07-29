namespace SuperCode
{
	public abstract class MemAst: Ast
	{
		public readonly Token semicol;

		public MemAst() { }
		public MemAst(Token semi) =>
			semicol = semi;
	}
}
