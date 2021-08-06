using LLVMSharp.Interop;

namespace SuperCode
{
	public enum UnOp
	{
		Neg,
		FNeg,
		Ref,
		Deref,
		Not,
	}

	public class UnExprNode: ExprNode
	{
		public readonly UnOp op;
		public readonly ExprNode expr;

		public UnExprNode(LLVMTypeRef type, UnOp op, ExprNode expr): base(type)
		{
			this.op = op;
			this.expr = expr;
		}

		public override bool mut => op is UnOp.Deref && expr.mut;
		public override NodeKind kind => NodeKind.UnExpr;
	}
}
