using LLVMSharp.Interop;

namespace SuperCode
{
	public class VarStmtNode: Node, Symbol
	{
		public readonly ExprNode init;

		public VarStmtNode(LLVMTypeRef ty, string name, ExprNode init)
		{
			this.type = ty;
			this.name = name;
			this.init = init;
		}

		public string name { get; init; }
		public bool unsigned { get; init; }
		public LLVMTypeRef type { get; init; }
		public override NodeKind kind => NodeKind.VarStmt;
	}
}
