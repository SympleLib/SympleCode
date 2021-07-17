using LLVMSharp.Interop;

namespace SuperCode
{
	public class VarStmtNode: StmtNode
	{
		public readonly LLVMTypeRef type;
		public readonly string name;
		public readonly ExprNode init;

		public VarStmtNode(LLVMTypeRef ty, string name, ExprNode init)
		{
			type = ty;
			this.name = name;
			this.init = init;
		}

		public override NodeKind kind => NodeKind.VarStmt;
	}
}
