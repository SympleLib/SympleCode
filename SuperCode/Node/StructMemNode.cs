using LLVMSharp.Interop;

namespace SuperCode
{
	public class StructMemNode: MemNode, Symbol
	{
		public readonly FieldNode[] fields;

		public StructMemNode(LLVMTypeRef type, string name, FieldNode[] fields)
		{
			this.type = type;
			this.name = name;
			this.fields = fields;
		}

		public override NodeKind kind => NodeKind.StructMem;
		public LLVMTypeRef type { get; }
		public string name { get; }
	}
}
