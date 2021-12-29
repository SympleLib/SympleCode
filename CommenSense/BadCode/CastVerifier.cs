namespace CommenSense;

using Type = LLVMTypeRef;
using Value = LLVMValueRef;

static class CastVerifier
{
	// Always for implicit casts
	public static bool CastWorks(Type from, Type to, out bool signExt)
	{
		signExt = false;
		
		if (from == Type.Void)
			return false;
		
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
		if (from.Kind is LLVMFloatTypeKind && !to.IsFloat())
			return false;
		return true;
	}
	
	
	public static int CastPrecedence(Type from, Type to)
	{
		if (from == Type.Void)
			return 0;
		
		if (from.Kind is LLVMIntegerTypeKind && to.Kind is LLVMIntegerTypeKind)
		{
			// cast to bool
			if (to.IntWidth == 1)
				return 1;

			// cast from bool
			if (from.IntWidth == 1)
				return 1;

			if (from.IntWidth > to.IntWidth)
				return 1;
		}

		if (from.Kind is LLVMDoubleTypeKind && to.Kind is LLVMFloatTypeKind)
			return 0;
		if (from.Kind is LLVMFloatTypeKind && !to.IsFloat())
			return 0;
		if (from.Kind == to.Kind)
			return 3;
		return 2;
	}
}