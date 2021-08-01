using LLVMSharp.Interop;

namespace SuperCode
{
	public class BadSymbol: Symbol
	{
		public bool mut => true;
		public string name => "Bad Symbol";
		public Visibility vis => Visibility.Default;
		public LLVMTypeRef type => null;
	}
}
