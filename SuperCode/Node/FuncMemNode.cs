namespace SuperCode
{
	public class FuncMemNode: MemNode
	{
		public readonly string name;
		public readonly StmtNode[] stmts;

		public FuncMemNode(string name, StmtNode[] stmts)
		{
			this.name = name;
			this.stmts = stmts;
		}

		public override NodeKind kind => NodeKind.FuncMem;
	}
}
