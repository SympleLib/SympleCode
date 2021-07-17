using LLVMSharp.Interop;

namespace SuperCode
{
	public static class LLVMALittleBitBald
	{
		public static bool IsFloat(this LLVMTypeRef type) =>
			type == LLVMTypeRef.BFloat || type == LLVMTypeRef.Half || type == LLVMTypeRef.Float || type == LLVMTypeRef.Double || type == LLVMTypeRef.Double;
	}
}
