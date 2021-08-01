using LLVMSharp.Interop;

namespace SuperCode
{
	public enum Visibility
	{
		Import,
		Private,
		Protected,
		Public,
		Export,

		Default = Public,
	}

	public interface Symbol
	{
		public bool mut { get; }
		public string name { get; }
		public Visibility vis { get; }
		public LLVMTypeRef type { get; }
	}
}
