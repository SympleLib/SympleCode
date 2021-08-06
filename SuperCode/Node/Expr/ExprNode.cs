using LLVMSharp.Interop;

namespace SuperCode
{
	public abstract class ExprNode: Node
	{
		public virtual bool mut => false;
		public readonly LLVMTypeRef type;

		public ExprNode(LLVMTypeRef type) =>
			this.type = type;
	}
}
