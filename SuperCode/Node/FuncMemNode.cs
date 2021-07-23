using LLVMSharp.Interop;

namespace SuperCode
{
	public class FuncMemNode: MemNode, Symbol
	{
		public readonly ParamNode[] paramz;
		public readonly StmtNode[] stmts;

		public FuncMemNode(TypeNode ty, string name, ParamNode[] paramz, StmtNode[] stmts)
		{
			this.type = ty;
			this.name = name;
			this.paramz = paramz;
			this.stmts = stmts;
		}

		public string name { get; init; }
		public bool unsigned { get; init; }
		public TypeNode type { get; init; }
		public override NodeKind kind => NodeKind.FuncMem;
	}
}
