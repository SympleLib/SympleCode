using System;
using System.Collections.Generic;

using LLVMSharp.Interop;

namespace SuperCode
{
	public static class LLVMALittleBitBald
	{
		public static readonly List<IntPtr> refTypes = new List<IntPtr>();

		public static LLVMBasicBlockRef AppendBasicBlock(this LLVMValueRef fn) =>
			fn.AppendBasicBlock(null);

		public static bool IsFloat(this LLVMTypeRef type) =>
			type == LLVMTypeRef.BFloat || type == LLVMTypeRef.Half || type == LLVMTypeRef.Float || type == LLVMTypeRef.Double || type == LLVMTypeRef.Double;

		public static bool IsPtr(this LLVMTypeRef type)
		{
			if (type.IsRef())
				return type.ElementType.ElementType != default;
			return type.ElementType != default;
		}

		public static LLVMTypeRef Ptr(this LLVMTypeRef type) =>
			LLVMTypeRef.CreatePointer(type, 0);

		public static LLVMTypeRef Ref(this LLVMTypeRef type)
		{
			var newType = LLVMTypeRef.CreatePointer(type, 0);
			refTypes.Add(newType.Handle);
			return newType;
		}

		public static bool IsRef(this LLVMTypeRef type) =>
			refTypes.Contains(type.Handle);

		public static LLVMValueRef AddGlobal(this LLVMModuleRef module, LLVMTypeRef ty) =>
			module.AddGlobal(ty, null);
	}
}
