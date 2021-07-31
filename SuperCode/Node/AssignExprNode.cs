namespace SuperCode
{
	public class AssignExprNode: ExprNode
	{
		public readonly ExprNode value;
		public readonly ExprNode expr;

		public AssignExprNode(ExprNode value, ExprNode expr): base(expr.type)
		{
			this.value = value;
			this.expr = expr;
		}

		public override NodeKind kind => NodeKind.AssignExpr;
	}
}
