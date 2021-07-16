namespace SuperCode
{
	public class VarExprNode: ExprNode
	{
		public readonly string name;

		public VarExprNode(string name) =>
			this.name = name;

		public override NodeKind kind => NodeKind.VarExpr;
	}
}
