using LLVMSharp.Interop;

namespace SuperCode
{
	public class TypePunExprNode: ExprNode
	{
		public readonly ExprNode expr;

		public TypePunExprNode(LLVMTypeRef type, ExprNode expr): base(type) =>
			this.expr = expr;

		public override NodeKind kind => NodeKind.TypePunExpr;
	}
}
