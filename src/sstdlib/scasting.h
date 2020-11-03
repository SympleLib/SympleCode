#ifndef _SSTDB_LIB_SCASTING_H_
#define _SSTDB_LIB_SCASTING_H_
#include "../svm.h"

namespace sstdlib
{
	sbyte CastToByte(sid);
	sshort CastToShort(sid);
	sint CastToInt(sid);
	slong CastToLong(sid);
	schar CastToChar(sid);
	sbool CastToBool(sid);
}
#endif