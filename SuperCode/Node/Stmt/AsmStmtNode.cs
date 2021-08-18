namespace SuperCode
{
	public class AsmStmtNode: Node
	{
		public readonly string asm;
		
		public AsmStmtNode(string asm) =>
			this.asm = asm;

		public override NodeKind kind => NodeKind.AsmStmt;
	}
}
