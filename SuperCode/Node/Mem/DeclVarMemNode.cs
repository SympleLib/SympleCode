using LLVMSharp.Interop;

namespace SuperCode
{
	public class DeclVarMemNode: Node, Symbol
	{
		public VarMemNode? impl { get; internal set; }

		public DeclVarMemNode(bool mut, LLVMTypeRef type, string name)
		{
			this.mut = mut;
			this.type = type;
			this.name = name;
		}

		public bool mut { get; }
		public string name { get; }
		public Visibility vis { get; }
		public LLVMTypeRef type { get; }
		public override NodeKind kind => NodeKind.DeclVarMem;
	}
}
