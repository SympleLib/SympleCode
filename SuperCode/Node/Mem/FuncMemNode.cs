using LLVMSharp.Interop;

namespace SuperCode
{
	public class FuncMemNode: Node, Symbol
	{
		public readonly bool noret;
		public readonly LLVMTypeRef retType;
		public readonly FieldNode[] paramz;
		public readonly Node[] stmts;

		public FuncMemNode(bool noret, LLVMTypeRef type, Visibility vis, string name, FieldNode[] paramz, Node[] stmts)
		{
			this.noret = noret;
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
