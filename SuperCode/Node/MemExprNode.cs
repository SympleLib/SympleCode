using LLVMSharp.Interop;

namespace SuperCode
{
	public class MemExprNode: ExprNode
	{
		public readonly ExprNode expr;
		public readonly FieldNode field;

		public MemExprNode(ExprNode expr, FieldNode field): base(field.type)
		{
			this.expr = expr;
			this.field = field;
		}

		public override bool mut => expr.mut && field.mut;
		public override NodeKind kind => NodeKind.MemExpr;
	}
}
