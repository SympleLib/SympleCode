using LLVMSharp.Interop;

namespace SuperCode
{
	public enum UnOp
	{
		Neg,
		Ref,
		Deref,
	}

	public class UnExprNode: ExprNode
	{
		public readonly UnOp op;
		public readonly ExprNode expr;

		public UnExprNode(LLVMTypeRef type, UnOp op, ExprNode expr)
		{
			this.type = type;
			this.op = op;
			this.expr = expr;
		}

		public override NodeKind kind => NodeKind.UnExpr;
	}
}
