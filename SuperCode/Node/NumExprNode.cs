using LLVMSharp.Interop;

namespace SuperCode
{
	public class NumExprNode: ExprNode
	{
		public readonly ulong value;

		public NumExprNode(ulong val)
		{
			value = val;
			type = LLVMTypeRef.Int32;
		}

		public override NodeKind kind => NodeKind.NumExpr;
	}
}
