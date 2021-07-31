namespace SuperCode
{
	public class IfStmtNode: Node
	{
		public readonly ExprNode cond;
		public readonly Node then;
		public readonly Node elze;

		public IfStmtNode(ExprNode cond, Node then, Node elze)
		{
			this.cond = cond;
			this.then = then;
			this.elze = elze;
		}

		public override NodeKind kind => NodeKind.IfStmt;
	}
}
