using LLVMSharp.Interop;

namespace SuperCode
{
	public class FuncMemNode: Node, Symbol
	{
		public readonly FieldNode[] paramz;
		public readonly Node[] stmts;

		public FuncMemNode(LLVMTypeRef ty, string name, FieldNode[] paramz, Node[] stmts)
		{
			this.type = ty;
			this.name = name;
			this.paramz = paramz;
			this.stmts = stmts;
		}

		public string name { get; }
		public LLVMTypeRef type { get; }
		public override NodeKind kind => NodeKind.FuncMem;
	}
}
