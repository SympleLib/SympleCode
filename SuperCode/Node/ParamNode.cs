using LLVMSharp.Interop;

namespace SuperCode
{
	public class ParamNode: Node, Symbol
	{
		public ParamNode(TypeNode ty, string name)
		{
			this.type = ty;
			this.name = name;
		}

		public string name { get; init; }
		public bool unsigned { get; init; }
		public TypeNode type { get; init; }
		public override NodeKind kind => NodeKind.Param;
	}
}
