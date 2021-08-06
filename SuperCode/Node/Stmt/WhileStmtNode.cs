namespace SuperCode
{
	public class WhileStmtNode: Node
	{
		public ExprNode cond;
		public Node then;

		public WhileStmtNode(ExprNode cond, Node then)
		{
			this.cond = cond;
			this.then = then;
		}

		public override NodeKind kind => NodeKind.WhileStmt;
	}
}
