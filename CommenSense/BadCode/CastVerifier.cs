namespace CommenSense;

using Type = LLVMTypeRef;
using Value = LLVMValueRef;

static class CastVerifier
{
	// Always for implicit casts
	public static bool CastWorks(Type from, Type to, out bool signExt)
	{
		signExt = false;
		if (from.Kind is LLVMIntegerTypeKind && to.Kind is LLVMIntegerTypeKind)
		{
			// cast to bool
			if (to.IntWidth == 1)
				return true;

			// cast from bool
			if (from.IntWidth == 1)
			{
				signExt = true;
				return true;
			}

			if (from.IntWidth > to.IntWidth)
				return false;
		}

		if (from.Kind is LLVMDoubleTypeKind && to.Kind is LLVMFloatTypeKind)
			return false;
		return true;
	}
}