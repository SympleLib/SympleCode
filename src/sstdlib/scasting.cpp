#include "scasting.h"

namespace sstdlib
{
	sbyte CastToByte(sid var)
	{
		sbyte casted = (sbyte)0;
		sid type = sGetVariebleClass(var);
		switch (type)
		{
		case BYTE_CLASS:
		{
			casted = (sbyte)sGetByteVarieble(var);
			break;
		}
		case SHORT_CLASS:
		{
			casted = (sbyte)sGetShortVarieble(var);
			break;
		}
		case INT_CLASS:
		{
			casted = (sbyte)sGetIntVarieble(var);
			break;
		}
		case LONG_CLASS:
		{
			casted = (sbyte)sGetLongVarieble(var);
			break;
		}
		case CHAR_CLASS:
		{
			casted = (sbyte)sGetCharVarieble(var);
			break;
		}
		case BOOL_CLASS:
		{
			casted = (sbyte)sGetBoolVarieble(var);
			break;
		}
		}

		return casted;
	}

	sshort CastToShort(sid var)
	{
		sshort casted = (sshort)0;
		sid type = sGetVariebleClass(var);
		switch (type)
		{
		case BYTE_CLASS:
		{
			casted = (sshort)sGetByteVarieble(var);
			break;
		}
		case SHORT_CLASS:
		{
			casted = (sshort)sGetShortVarieble(var);
			break;
		}
		case INT_CLASS:
		{
			casted = (sshort)sGetIntVarieble(var);
			break;
		}
		case LONG_CLASS:
		{
			casted = (sshort)sGetLongVarieble(var);
			break;
		}
		case CHAR_CLASS:
		{
			casted = (sshort)sGetCharVarieble(var);
			break;
		}
		case BOOL_CLASS:
		{
			casted = (sshort)sGetBoolVarieble(var);
			break;
		}
		}

		return casted;
	}

	sint CastToInt(sid var)
	{
		sint casted = (sint)0;
		sid type = sGetVariebleClass(var);
		switch (type)
		{
		case BYTE_CLASS:
		{
			casted = (sint)sGetByteVarieble(var);
			break;
		}
		case SHORT_CLASS:
		{
			casted = (sint)sGetShortVarieble(var);
			break;
		}
		case INT_CLASS:
		{
			casted = (sint)sGetIntVarieble(var);
			break;
		}
		case LONG_CLASS:
		{
			casted = (sint)sGetLongVarieble(var);
			break;
		}
		case CHAR_CLASS:
		{
			casted = (sint)sGetCharVarieble(var);
			break;
		}
		case BOOL_CLASS:
		{
			casted = (sint)sGetBoolVarieble(var);
			break;
		}
		}

		return casted;
	}

	slong CastToLong(sid var)
	{
		slong casted = (slong)0;
		sid type = sGetVariebleClass(var);
		switch (type)
		{
		case BYTE_CLASS:
		{
			casted = (slong)sGetByteVarieble(var);
			break;
		}
		case SHORT_CLASS:
		{
			casted = (slong)sGetShortVarieble(var);
			break;
		}
		case INT_CLASS:
		{
			casted = (slong)sGetIntVarieble(var);
			break;
		}
		case LONG_CLASS:
		{
			casted = (slong)sGetLongVarieble(var);
			break;
		}
		case CHAR_CLASS:
		{
			casted = (slong)sGetCharVarieble(var);
			break;
		}
		case BOOL_CLASS:
		{
			casted = (slong)sGetBoolVarieble(var);
			break;
		}
		}

		return casted;
	}

	schar CastToChar(sid var)
	{
		schar casted = (schar)0;
		sid type = sGetVariebleClass(var);
		switch (type)
		{
		case BYTE_CLASS:
		{
			casted = (schar)sGetByteVarieble(var);
			break;
		}
		case SHORT_CLASS:
		{
			casted = (schar)sGetShortVarieble(var);
			break;
		}
		case INT_CLASS:
		{
			casted = (schar)sGetIntVarieble(var);
			break;
		}
		case LONG_CLASS:
		{
			casted = (schar)sGetLongVarieble(var);
			break;
		}
		case CHAR_CLASS:
		{
			casted = (schar)sGetCharVarieble(var);
			break;
		}
		case BOOL_CLASS:
		{
			casted = (schar)sGetBoolVarieble(var);
			break;
		}
		}

		return casted;
	}

	sbool CastToBool(sid var)
	{
		sbool casted = (sbool)0;
		sid type = sGetVariebleClass(var);
		switch (type)
		{
		case BYTE_CLASS:
		{
			casted = (sbool)sGetByteVarieble(var);
			break;
		}
		case SHORT_CLASS:
		{
			casted = (sbool)sGetShortVarieble(var);
			break;
		}
		case INT_CLASS:
		{
			casted = (sbool)sGetIntVarieble(var);
			break;
		}
		case LONG_CLASS:
		{
			casted = (sbool)sGetLongVarieble(var);
			break;
		}
		case CHAR_CLASS:
		{
			casted = (sbool)sGetCharVarieble(var);
			break;
		}
		case BOOL_CLASS:
		{
			casted = (sbool)sGetBoolVarieble(var);
			break;
		}
		}

		return casted;
	}
}