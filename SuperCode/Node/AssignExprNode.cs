namespace SuperCode
{
	public class AssignExprNode: ExprNode
	{
		public readonly ExprNode expr;
		public readonly ExprNode value;

		public AssignExprNode(ExprNode expr, ExprNode value): base(expr.type)
		{
			this.expr = expr;
			this.value = value;
		}

		public override NodeKind kind => NodeKind.AssignExpr;
	}
}
