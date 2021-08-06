using LLVMSharp.Interop;

namespace SuperCode
{
	public class ArrExprNode: ExprNode
	{
		public readonly ExprNode[] elements;

		public ArrExprNode(LLVMTypeRef type, ExprNode[] elements): base(type) =>
			this.elements = elements;

		public override NodeKind kind => NodeKind.ArrExpr;
	}
}
