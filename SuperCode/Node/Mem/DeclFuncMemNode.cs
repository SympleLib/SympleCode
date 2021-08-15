using LLVMSharp.Interop;

namespace SuperCode
{
	public class DeclFuncMemNode: Node, Symbol
	{
		public readonly bool noret;
		public readonly LLVMTypeRef retType;
		public readonly FieldNode[] paramz;
		public FuncMemNode? impl { get; internal set; }

		public DeclFuncMemNode(bool noret, LLVMTypeRef type, string name, FieldNode[] paramz)
		{
			this.noret = noret;
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
