using LLVMSharp.Interop;

namespace SuperCode
{
	public static class LLVMALittleBitBald
	{
		public static LLVMBasicBlockRef AppendBasicBlock(this LLVMValueRef fn) =>
			fn.AppendBasicBlock("");

		public static bool IsFloat(this LLVMTypeRef type) =>
			type == LLVMTypeRef.BFloat || type == LLVMTypeRef.Half || type == LLVMTypeRef.Float || type == LLVMTypeRef.Double || type == LLVMTypeRef.Double;

		public static bool IsPtr(this LLVMTypeRef type) =>
			type.ElementType != null;

		public static LLVMTypeRef Ref(this LLVMTypeRef type) =>
			LLVMTypeRef.CreatePointer(type, 0);

		public static LLVMTypeRef Deref(this LLVMTypeRef type) =>
			type.ElementType;
	}
}
