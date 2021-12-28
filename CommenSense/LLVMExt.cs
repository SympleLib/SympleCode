namespace CommenSense;

using Type = LLVMTypeRef;
using Value = LLVMValueRef;
using static LLVMRealPredicate;

static class LLVMExt
{
	public static bool IsFloat(this Type type) =>
		type == Type.Half || type == Type.Float || type == Type.Double ||
		type == Type.BFloat || type == Type.X86FP80;

	public static bool IsPtr(this Type type) =>
		type.ElementType != default;

	public static LLVMRealPredicate ToRealPredicate(this LLVMIntPredicate ip) =>
		ip switch
		{
			LLVMIntEQ => LLVMRealOEQ,
			LLVMIntNE => LLVMRealONE,
			LLVMIntSLT => LLVMRealOLT,
			LLVMIntSLE => LLVMRealOLE,
			LLVMIntSGT => LLVMRealOGT,
			LLVMIntSGE => LLVMRealOGE,
			
			_ => throw new NotImplementedException(),
		};
}