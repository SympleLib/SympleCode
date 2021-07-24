using LLVMSharp.Interop;

namespace SuperCode
{
	public abstract class ExprNode: StmtNode
	{ public LLVMTypeRef type { get; protected init; } = LLVMTypeRef.Int32; }
}
