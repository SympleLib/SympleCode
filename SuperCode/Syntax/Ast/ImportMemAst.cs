namespace SuperCode
{
	public class ImportMemAst: MemAst
	{
		public readonly Token key;
		public readonly Token what;

		public ImportMemAst(Token key, Token what, Token semi): base(semi)
		{
			this.key = key;
			this.what = what;
		}

		public override AstKind kind => AstKind.ImportMem;
	}
}
