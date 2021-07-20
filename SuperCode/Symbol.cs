using LLVMSharp.Interop;

namespace SuperCode
{
	public interface Symbol
	{
		public string name { get; init; }
		public bool unsigned { get; init; }
		public LLVMTypeRef type { get; init; }
	}
}
