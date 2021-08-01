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

		public bool mut { get; }
		public string name { get; }
		public Visibility vis => Visibility.Default;
		public LLVMTypeRef type { get; }
		public override NodeKind kind => NodeKind.StructMem;
	}
}
