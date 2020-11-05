#pragma once

#include <stdint.h>
#include <string>

typedef unsigned __int32 sid;
typedef sid svarid;
typedef sid sfuncid;
typedef sid stypeid;
typedef sid sclassid;

typedef unsigned __int32 ssize;
typedef __int8 sbyte;
typedef __int16 sshort;
typedef __int32 sint;
typedef __int64 slong;
typedef char schar;
typedef bool sbool;
typedef std::string sstring;
typedef class {}* sobject;

typedef unsigned __int8 subyte;
typedef unsigned __int16 sushort;
typedef unsigned __int32 suint;
typedef unsigned __int64 sulong;

#define NONE_TYPE   0x00
#define BYTE_TYPE   0x01
#define SHORT_TYPE  0x02
#define INT_TYPE    0x03
#define LONG_TYPE   0x04
#define CHAR_TYPE   0x05
#define BOOL_TYPE   0x06
#define OBJECT_TYPE 0x07

/*

switch (type)
{
case BYTE_TYPE:
	break;
case SHORT_TYPE:
	break;
case INT_TYPE:
	break;
case LONG_TYPE:
	break;
case BOOL_TYPE:
	break;
case OBJECT_TYPE:
	break;
}

*/