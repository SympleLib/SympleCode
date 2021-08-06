using LLVMSharp.Interop;

namespace SuperCode
{
	public class FieldExprNode: ExprNode
	{
		public readonly ExprNode expr;
		public readonly FieldNode field;

		public FieldExprNode(ExprNode expr, FieldNode field): base(field.type)
		{
			this.expr = expr;
			this.field = field;
		}

		public override bool mut => expr.mut && field.mut;
		public override NodeKind kind => NodeKind.MemExpr;
	}
}
