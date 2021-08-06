using LLVMSharp.Interop;

namespace SuperCode
{
	public class VarStmtNode: Node, Symbol
	{
		public readonly ExprNode? init;

		public VarStmtNode(LLVMTypeRef ty, bool mut, string name, ExprNode? init)
		{
			this.type = ty;
			this.mut = mut;
			this.name = name;
			this.init = init;
		}

		public bool mut { get; }
		public string name { get; }
		public Visibility vis => Visibility.Default;
		public LLVMTypeRef type { get; }
		public override NodeKind kind => NodeKind.VarStmt;
	}
}
