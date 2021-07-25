using LLVMSharp.Interop;

namespace SuperCode
{
	public class DeclFuncMemNode: MemNode, Symbol
	{
		public readonly FieldNode[] paramz;

		public DeclFuncMemNode(LLVMTypeRef type, string name, FieldNode[] paramz)
		{
			this.type = type;
			this.name = name;
			this.paramz = paramz;
		}

		public string name { get; }
		public LLVMTypeRef type { get; }
		public override NodeKind kind => NodeKind.DeclFuncMem;
	}
}
