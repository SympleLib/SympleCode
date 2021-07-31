using LLVMSharp.Interop;

namespace SuperCode
{
	public class DeclFuncMemNode: Node, Symbol
	{
		public LLVMTypeRef retType;
		public readonly FieldNode[] paramz;

		public DeclFuncMemNode(LLVMTypeRef type, string name, FieldNode[] paramz)
		{
			this.type = type;
			this.retType = type.ReturnType;
			this.name = name;
			this.paramz = paramz;
		}

		public bool mut { get; }
		public string name { get; }
		public LLVMTypeRef type { get; }
		public override NodeKind kind => NodeKind.DeclFuncMem;
	}
}
