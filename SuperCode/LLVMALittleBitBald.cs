using LLVMSharp.Interop;

namespace SuperCode
{
	public static class LLVMALittleBitBald
	{
		public static LLVMBasicBlockRef AppendBasicBlock(this LLVMValueRef fn) =>
			fn.AppendBasicBlock(null);

		public static bool IsFloat(this LLVMTypeRef type) =>
			type == LLVMTypeRef.BFloat || type == LLVMTypeRef.Half || type == LLVMTypeRef.Float || type == LLVMTypeRef.Double || type == LLVMTypeRef.Double;

		public static bool IsPtr(this LLVMTypeRef type) =>
			type.ElementType != null;

		public static LLVMTypeRef Ref(this LLVMTypeRef type) =>
			LLVMTypeRef.CreatePointer(type, 0);

		public static LLVMValueRef AddGlobal(this LLVMModuleRef module, LLVMTypeRef ty) =>
			module.AddGlobal(ty, null);
	}
}
