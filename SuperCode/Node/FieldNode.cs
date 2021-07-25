using LLVMSharp.Interop;

namespace SuperCode
{
	public class FieldNode: Node, Symbol
	{
		public FieldNode(LLVMTypeRef ty, string name)
		{
			this.type = ty;
			this.name = name;
		}

		public string name { get; }
		public LLVMTypeRef type { get; }
		public override NodeKind kind => NodeKind.Field;
	}
}
