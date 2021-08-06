using LLVMSharp.Interop;

namespace SuperCode
{
	public class CastExprNode: ExprNode
	{
		public readonly ExprNode value;

		public CastExprNode(ExprNode val, LLVMTypeRef to): base(to) =>
			value = val;
		
		public override NodeKind kind => NodeKind.CastExpr;
	}
}
