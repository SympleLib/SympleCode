using LLVMSharp.Interop;

namespace SuperCode
{
	public class TypedefStmtNode: Node
	{
		public string alias;
		public LLVMTypeRef real;

		public TypedefStmtNode(string alias, LLVMTypeRef real)
		{
			this.alias = alias;
			this.real = real;
		}

		public override NodeKind kind => NodeKind.TypedefStmt;
	}
}
