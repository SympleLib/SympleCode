namespace SuperCode
{
	public class ModuleAst: Ast
	{
		public string filename;
		public readonly MemAst[] mems;
		public readonly Token eof;

		public ModuleAst(string file, MemAst[] mems, Token eof)
		{
			token = eof;
			filename = file;
			this.mems = mems;
			this.eof = eof;
		}

		public override AstKind kind => AstKind.Module;
	}
}
