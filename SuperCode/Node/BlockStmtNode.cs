namespace SuperCode
{
	public class BlockStmtNode: Node
	{
		public Node[] stmts;

		public BlockStmtNode(Node[] stmts) =>
			this.stmts = stmts;

		public override NodeKind kind => NodeKind.BlockStmt;
	}
}
