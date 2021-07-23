using LLVMSharp.Interop;
using System.Collections.Generic;

namespace SuperCode
{
	public static class BuiltinTypes
	{
		// Subject to change...
		public static readonly Dictionary<string, TypeNode> types = new()
		{
			{ "void", TypeNode.v },

			{ "sbyte", TypeNode.i8 },
			{ "short", TypeNode.i16 },
			{ "int", TypeNode.i32 },
			{ "long", TypeNode.i64 },

			{ "byte", TypeNode.u8 },
			{ "ushort", TypeNode.u16 },
			{ "uint", TypeNode.u32 },
			{ "ulong", TypeNode.u64 },
			
			{ "float", TypeNode.fp32 },
			{ "bfloat", TypeNode.fb16 },

			{ "half", TypeNode.fp16 },
			{ "single", TypeNode.fp32 },
			{ "double", TypeNode.fp64 },
			{ "triple", TypeNode.fp128 },

			{ "char", TypeNode.i16 },
			{ "schar", TypeNode.i8 },
			{ "wchar", TypeNode.i32 },
			{ "bool", TypeNode.i1 },

			{ "bit", TypeNode.i1 },
			{ "word", TypeNode.u16 },
			{ "dword", TypeNode.u32 },
			{ "qword", TypeNode.u64 },



			{ "bf16", TypeNode.fb16 },
			{ "fp16", TypeNode.fp16 },
			{ "fp32", TypeNode.fp32 },
			{ "fp64", TypeNode.fp64 },
			{ "fp80", TypeNode.fp80 },
			{ "fp128", TypeNode.fp128 },


			{ "i1", TypeNode.i1 },
			{ "i8", TypeNode.i8 },
			{ "i16", TypeNode.i16 },
			{ "i32", TypeNode.i32 },
			{ "i64", TypeNode.i64 },

			{ "u8", TypeNode.u8 },
			{ "u16", TypeNode.u16 },
			{ "u32", TypeNode.u32 },
			{ "u64", TypeNode.u64 },


			{ "i1", TypeNode.i1 },
			{ "i8", TypeNode.i8 },
			{ "i16", TypeNode.i16 },
			{ "i32", TypeNode.i32 },
			{ "i64", TypeNode.i64 },
		};


		public static readonly Dictionary<string, bool> utypes = new()
		{
			{ "void", false },

			{ "sbyte", false },
			{ "short", false },
			{ "i", false },
			{ "long", false },

			{ "byte", true },
			{ "ushort", true },
			{ "ui", true },
			{ "ulong", true },

			{ "float", false },
			{ "bfloat", false },

			{ "half", false },
			{ "single", false },
			{ "double", false },
			{ "triple", false },

			{ "char", false },
			{ "schar", false },
			{ "wchar", false },
			{ "bool", false },



			{ "bf16", false },
			{ "fp16", false },
			{ "fp32", false },
			{ "fp64", false },
			{ "fp80", false },
			{ "fp128", false },


			{ "i1", false },
			{ "i8", false },
			{ "i16", false },
			{ "i32", false },
			{ "i64", false },

			{ "u1", true },
			{ "u8", true },
			{ "u16", true },
			{ "u32", true },
			{ "u64", true },


			{ "i1", false },
			{ "i8", false },
			{ "i16", false },
			{ "i32", false },
			{ "i64", false },

			{ "ui1", true },
			{ "ui8", true },
			{ "ui16", true },
			{ "ui32", true },
			{ "ui64", true },

			{ "word", true },
			{ "dword", true },
			{ "qword", true },
		};
	}
}
