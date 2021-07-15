namespace SuperCode
{
	public class ModuleAst: Ast
	{
		public readonly MemAst[] mems;
		public readonly Token eof;

		public ModuleAst(MemAst[] mems, Token eof)
		{
			this.mems = mems;
			this.eof = eof;
		}

		public override AstKind kind => AstKind.Module;
	}
}
