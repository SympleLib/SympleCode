namespace CommenSense;

using Type = LLVMTypeRef;
using Value = LLVMValueRef;

static class LLVMExt
{
	public static bool IsFloat(this Type type) =>
		type == Type.Half || type == Type.Float || type == Type.Double ||
		type == Type.BFloat || type == Type.X86FP80;

	public static bool IsPtr(this Type type) =>
		type.ElementType != default;
}