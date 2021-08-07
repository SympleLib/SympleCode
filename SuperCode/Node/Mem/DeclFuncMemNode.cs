using LLVMSharp.Interop;

namespace SuperCode
{
	public class DeclFuncMemNode: Node, Symbol
	{
		public readonly LLVMTypeRef retType;
		public FieldNode[]? paramz {  get; private set; }
		public FuncMemNode? impl { get; private set; }

		public DeclFuncMemNode(LLVMTypeRef type, string name)
		{
			this.type = type;
			this.retType = type.ReturnType;
			this.name = name;
		}

		public void Complete(FieldNode[] paramz, FuncMemNode? impl)
		{
			this.paramz = paramz;
			this.impl = impl;
		}

		public bool mut => false;
		public string name { get; }
		public Visibility vis => Visibility.Default;
		public LLVMTypeRef type { get; }
		public override NodeKind kind => NodeKind.DeclFuncMem;
	}
}
