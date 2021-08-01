using System;
using System.Collections.Generic;

using LLVMSharp.Interop;
using static LLVMSharp.Interop.LLVMLinkage;

namespace SuperCode
{
	public static class LLVMALittleBitBald
	{
		public static readonly LLVMTypeRef uint8 = LLVMTypeRef.CreateInt(8);
		public static readonly LLVMTypeRef uint16 = LLVMTypeRef.CreateInt(16);
		public static readonly LLVMTypeRef uint32 = LLVMTypeRef.CreateInt(32);
		public static readonly LLVMTypeRef uint64 = LLVMTypeRef.CreateInt(64);

		public static readonly List<IntPtr> refTypes = new List<IntPtr>();
		public static readonly List<IntPtr> usTypes = new List<IntPtr>()
		{ uint8.Handle, uint16.Handle, uint32.Handle, uint64.Handle, };

		public static LLVMBasicBlockRef AppendBasicBlock(this LLVMValueRef fn) =>
			fn.AppendBasicBlock(null);

		public static bool IsUnsigned(this LLVMTypeRef type) =>
			usTypes.Contains(type.Handle);

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

		public static void Apply(this Visibility vis, LLVMValueRef val)
		{
			if (vis is Visibility.Protected)
				val.Linkage = LLVMInternalLinkage;
			if (vis is Visibility.Private)
				val.Linkage = LLVMPrivateLinkage;
			if (vis is Visibility.Public)
				val.Linkage = LLVMExternalLinkage;
			if (vis is Visibility.Import)
				val.Linkage = LLVMDLLImportLinkage;
			if (vis is Visibility.Export)
				val.Linkage = LLVMDLLExportLinkage;
		}
	}
}
