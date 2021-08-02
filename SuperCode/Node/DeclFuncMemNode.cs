using LLVMSharp.Interop;

namespace SuperCode
{
	public class DeclFuncMemNode: Node, Symbol
	{
		public readonly LLVMTypeRef retType;
		public readonly FieldNode[] paramz;
		public FuncMemNode impl { get; internal set; }

		public DeclFuncMemNode(LLVMTypeRef type, string name, FieldNode[] paramz)
		{
			this.type = type;
			this.retType = type.ReturnType;
			this.name = name;
			this.paramz = paramz;
		}

		public bool mut => false;
		public string name { get; }
		public Visibility vis => Visibility.Default;
		public LLVMTypeRef type { get; }
		public override NodeKind kind => NodeKind.DeclFuncMem;
	}
}
