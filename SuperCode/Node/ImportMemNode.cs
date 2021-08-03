namespace SuperCode
{
	public class ImportMemNode: Node
	{
		public string what;
		public ModuleNode module;

		public ImportMemNode(string what, ModuleNode module)
		{
			this.what = what;
			this.module = module;
		}

		public override NodeKind kind => NodeKind.ImportMem;
	}
}
