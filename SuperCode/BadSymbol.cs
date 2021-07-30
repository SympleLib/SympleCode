using LLVMSharp.Interop;

namespace SuperCode
{
	public class BadSymbol: Symbol
	{
		public string name => "Bad Symbol";
		public LLVMTypeRef type => null;
	}
}
