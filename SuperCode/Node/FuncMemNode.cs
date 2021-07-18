using LLVMSharp.Interop;

namespace SuperCode
{
	public class FuncMemNode: MemNode
	{
		public readonly LLVMTypeRef type;
		public readonly string name;
		public readonly StmtNode[] stmts;

		public FuncMemNode(LLVMTypeRef ty, string name, StmtNode[] stmts)
		{
			this.type = ty;
			this.name = name;
			this.stmts = stmts;
		}

		public override NodeKind kind => NodeKind.FuncMem;
	}
}
