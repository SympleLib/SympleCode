using LLVMSharp.Interop;

namespace SuperCode
{
	public interface Symbol
	{
		public bool mut { get; }
		public string name { get; }
		public LLVMTypeRef type { get; }
	}
}
