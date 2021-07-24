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

			{ "sbyte", LLVMTypeRef.Int8 },
			{ "short", LLVMTypeRef.Int16 },
			{ "int", LLVMTypeRef.Int32 },
			{ "long", LLVMTypeRef.Int64 },

			{ "byte", LLVMTypeRef.Int8 },
			{ "ushort", LLVMTypeRef.Int16 },
			{ "uint", LLVMTypeRef.Int32 },
			{ "ulong", LLVMTypeRef.Int64 },

			{ "float", LLVMTypeRef.Float },
			{ "bfloat", LLVMTypeRef.BFloat },

			{ "half", LLVMTypeRef.Half },
			{ "single", LLVMTypeRef.Float },
			{ "double", LLVMTypeRef.Double },
			{ "triple", LLVMTypeRef.FP128 },

			{ "char", LLVMTypeRef.Int16 },
			{ "schar", LLVMTypeRef.Int8 },
			{ "wchar", LLVMTypeRef.Int32 },
			{ "bool", LLVMTypeRef.Int1 },



			{ "bf16", LLVMTypeRef.BFloat },
			{ "fp16", LLVMTypeRef.Half },
			{ "fp32", LLVMTypeRef.Float },
			{ "fp64", LLVMTypeRef.Double },
			{ "fp80", LLVMTypeRef.X86FP80 },
			{ "fp128", LLVMTypeRef.FP128 },


			{ "i1", LLVMTypeRef.Int1 },
			{ "i8", LLVMTypeRef.Int8 },
			{ "i16", LLVMTypeRef.Int16 },
			{ "i32", LLVMTypeRef.Int32 },
			{ "i64", LLVMTypeRef.Int64 },

			{ "u1", LLVMTypeRef.Int1 },
			{ "u8", LLVMTypeRef.Int8 },
			{ "u16", LLVMTypeRef.Int16 },
			{ "u32", LLVMTypeRef.Int32 },
			{ "u64", LLVMTypeRef.Int64 },


			{ "int1", LLVMTypeRef.Int1 },
			{ "int8", LLVMTypeRef.Int8 },
			{ "int16", LLVMTypeRef.Int16 },
			{ "int32", LLVMTypeRef.Int32 },
			{ "int64", LLVMTypeRef.Int64 },

			{ "uint1", LLVMTypeRef.Int1 },
			{ "uint8", LLVMTypeRef.Int8 },
			{ "uint16", LLVMTypeRef.Int16 },
			{ "uint32", LLVMTypeRef.Int32 },
			{ "uint64", LLVMTypeRef.Int64 },

			{ "word", LLVMTypeRef.Int16 },
			{ "dword", LLVMTypeRef.Int32 },
			{ "qword", LLVMTypeRef.Int64 },
		};
	}
}
