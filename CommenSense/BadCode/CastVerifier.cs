namespace CommenSense;

using Type = LLVMTypeRef;
using Value = LLVMValueRef;

static class CastVerifier
{
	// Always for implicit casts
	public static bool CastWorks(Type from, Type to)
	{
		if (from.Kind is LLVMIntegerTypeKind && to.Kind is LLVMIntegerTypeKind)
		{
			// cast to bool
			if (to.IntWidth == 1)
				return true;

			if (from.IntWidth != to.IntWidth)
				return false;
		}

		// TODO: add more
		return true;
	}
}