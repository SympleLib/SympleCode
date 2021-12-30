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

	public static bool SameAs(this Type t1, Type t2)
	{
		if (t1.Kind != t2.Kind)
			return false;
		if (t1 == t2)
			return true;
		if (t1.ElementType == t2.ElementType)
			return true;
		return SameAs(t1.ElementType, t2.ElementType);
	}
}