namespace SuperCode
{
	public class RetStmtNode: Node
	{
		public readonly ExprNode? value;

		public RetStmtNode(ExprNode? val) =>
			value = val;

		public override NodeKind kind => NodeKind.RetStmt;
	}
}
