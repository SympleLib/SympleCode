using LLVMSharp.Interop;

namespace SuperCode
{
	public abstract class ExprNode: StmtNode
	{ public TypeNode type { get; protected init; } = TypeNode.i32; }
}
