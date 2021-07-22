using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using LLVMSharp.Interop;

namespace SuperCode
{
	public class TypeNode
	{
		public LLVMTypeRef lltype;
		public bool unsigned;

		public TypeNode(LLVMTypeRef lltype, bool unsigned)
		{
			this.lltype = lltype;
			this.unsigned = unsigned;
		}

		public static readonly TypeNode i1 = new TypeNode(LLVMTypeRef.Int1, false);
		public static readonly TypeNode i8 = new TypeNode(LLVMTypeRef.Int8, false);
		public static readonly TypeNode i16 = new TypeNode(LLVMTypeRef.Int16, false);
		public static readonly TypeNode i32 = new TypeNode(LLVMTypeRef.Int32, false);
		public static readonly TypeNode i64 = new TypeNode(LLVMTypeRef.Int64, false);

		public static readonly TypeNode u8 = new TypeNode(LLVMTypeRef.Int8, true);
		public static readonly TypeNode u16 = new TypeNode(LLVMTypeRef.Int16, true);
		public static readonly TypeNode u32 = new TypeNode(LLVMTypeRef.Int32, true);
		public static readonly TypeNode u64 = new TypeNode(LLVMTypeRef.Int64, true);

		public static readonly TypeNode fb16 = new TypeNode(LLVMTypeRef.BFloat, false);
		public static readonly TypeNode fp16 = new TypeNode(LLVMTypeRef.Half, false);
		public static readonly TypeNode fp32 = new TypeNode(LLVMTypeRef.Float, false);
		public static readonly TypeNode fp64 = new TypeNode(LLVMTypeRef.Double, false);
		public static readonly TypeNode fp80 = new TypeNode(LLVMTypeRef.X86FP80, false);
		public static readonly TypeNode fp128 = new TypeNode(LLVMTypeRef.FP128, false);
	}
}
