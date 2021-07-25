using LLVMSharp.Interop;

namespace SuperCode
{
	public interface Symbol
	{
		public string name { get; }
		public LLVMTypeRef type { get; }
	}
}
