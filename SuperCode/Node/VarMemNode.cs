using LLVMSharp.Interop;

namespace SuperCode
{
	public class VarMemNode: Node, Symbol
	{
		public readonly ExprNode init;

		public VarMemNode(LLVMTypeRef ty, string name, ExprNode init)
		{
			this.type = ty;
			this.name = name;
			this.init = init;
		}

		public bool mut { get; }
		public string name { get; }
		public LLVMTypeRef type { get; }
		public override NodeKind kind => NodeKind.VarMem;
	}
}
