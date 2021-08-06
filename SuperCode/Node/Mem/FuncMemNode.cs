using LLVMSharp.Interop;

namespace SuperCode
{
	public class FuncMemNode: Node, Symbol
	{
		public LLVMTypeRef retType;
		public readonly FieldNode[] paramz;
		public readonly Node[] stmts;

		public FuncMemNode(LLVMTypeRef type, Visibility vis, string name, FieldNode[] paramz, Node[] stmts)
		{
			this.type = type;
			this.vis = vis;
			this.retType = type.ReturnType;
			this.name = name;
			this.paramz = paramz;
			this.stmts = stmts;
		}

		public bool mut => false;
		public string name { get; }
		public Visibility vis { get; }
		public LLVMTypeRef type { get; }
		public override NodeKind kind => NodeKind.FuncMem;
	}
}
