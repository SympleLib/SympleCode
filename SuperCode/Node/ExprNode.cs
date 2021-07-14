using LLVMSharp.Interop;

namespace SuperCode
{
	public abstract class ExprNode: Node
	{ public LLVMTypeRef type { get; protected set; } = LLVMTypeRef.Int32; }
}
