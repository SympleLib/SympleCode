using LLVMSharp.Interop;

namespace SuperCode
{
	public class BadSymbol: Symbol
	{
		public bool mut => true;
		public string name => "Bad Symbol";
		public LLVMTypeRef type => null;
	}
}
