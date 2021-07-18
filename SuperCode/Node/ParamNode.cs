using LLVMSharp.Interop;

namespace SuperCode
{
	public class ParamNode: Node, Symbol
	{
		public ParamNode(LLVMTypeRef ty, string name)
		{
			this.type = ty;
			this.name = name;
		}

		public string name { get; init; }
		public LLVMTypeRef type { get; init; }
		public override NodeKind kind => NodeKind.Param;
	}
}
