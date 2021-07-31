using LLVMSharp.Interop;

namespace SuperCode
{
	public class StructMemNode: Node, Symbol
	{
		public readonly FieldNode[] fields;

		public StructMemNode(LLVMTypeRef type, string name, FieldNode[] fields)
		{
			this.type = type;
			this.name = name;
			this.fields = fields;
		}

		public string name { get; }
		public LLVMTypeRef type { get; }
		public override NodeKind kind => NodeKind.StructMem;
	}
}
