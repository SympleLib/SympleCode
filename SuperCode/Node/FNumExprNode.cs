using LLVMSharp.Interop;

namespace SuperCode
{
	public class FNumExprNode: ExprNode
	{
		public readonly double value;

		public FNumExprNode(double val, LLVMTypeRef ty): base(ty) =>
			value = val;

		public override NodeKind kind => NodeKind.FNumExpr;
	}
}
