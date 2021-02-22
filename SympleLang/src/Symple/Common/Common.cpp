#include "Symple/Common/Common.h"

bool StartsWith(const char *str, const char *start)
{
	uint32_t strLen = strlen(str), startLen = strlen(start);
	bool sz = strLen >= startLen;
	return sz && !strncmp(str, start, startLen);
}