using LLVMSharp.Interop;

namespace SuperCode
{
	public class FuncMemNode: MemNode
	{
		public readonly LLVMTypeRef retType;
		public readonly string name;
		public readonly StmtNode[] stmts;

		public FuncMemNode(LLVMTypeRef retType, string name, StmtNode[] stmts)
		{
			this.retType = retType;
			this.name = name;
			this.stmts = stmts;
		}

		public override NodeKind kind => NodeKind.FuncMem;
	}
}
