using LLVMSharp.Interop;
using System.Collections.Generic;

namespace SuperCode
{
	public static class BuiltinTypes
	{
		// Subject to change...
		public static readonly Dictionary<string, LLVMTypeRef> types = new()
		{
			{ "void", LLVMTypeRef.Void },
			{ "bit", LLVMTypeRef.Int8 },

			{ "sbyte", LLVMTypeRef.Int8 },
			{ "short", LLVMTypeRef.Int16 },
			{ "int", LLVMTypeRef.Int32 },
			{ "long", LLVMTypeRef.Int64 },

			{ "byte", LLVMALittleBitBald.uint8 },
			{ "ushort", LLVMALittleBitBald.uint16 },
			{ "uint", LLVMALittleBitBald.uint32 },
			{ "ulong", LLVMALittleBitBald.uint64 },


			{ "half", LLVMTypeRef.Half },
			{ "float", LLVMTypeRef.Float },
			{ "bfloat", LLVMTypeRef.BFloat },
			{ "double", LLVMTypeRef.Double },
			{ "triple", LLVMTypeRef.FP128 },

			{ "char", LLVMTypeRef.Int16 },
			{ "schar", LLVMTypeRef.Int8 },
			{ "wchar", LLVMTypeRef.Int32 },
			{ "bool", LLVMTypeRef.Int1 },
		};
	}
}
