using LLVMSharp.Interop;

namespace SuperCode
{
	public class VarMemNode: Node, Symbol
	{
		public readonly ExprNode init;

		public VarMemNode(LLVMTypeRef ty, Visibility vis, bool mut, string name, ExprNode init)
		{
			this.type = ty;
			this.vis = vis;
			this.mut = mut;
			this.name = name;
			this.init = init;
		}

		public bool mut { get; }
		public string name { get; }
		public Visibility vis { get; }
		public LLVMTypeRef type { get; }
		public override NodeKind kind => NodeKind.VarMem;
	}
}
