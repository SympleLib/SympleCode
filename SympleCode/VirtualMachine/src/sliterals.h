#pragma once

#include <stdint.h>
#include <string>

typedef unsigned int sid;
typedef sid svarid;
typedef sid sfuncid;
typedef sid stypeid;
typedef sid sclassid;

typedef __int8 sbyte;
typedef __int16 sshort;
typedef __int32 sint;
typedef __int64 slong;
typedef char schar;
typedef bool sbool;
typedef std::string sstring;
typedef class {}* sobject;

#define NONE_TYPE 0x0
#define BYTE_TYPE 0x1
#define SHORT_TYPE 0x2
#define INT_TYPE 0x3
#define LONG_TYPE 0x4
#define CHAR_TYPE 0x5
#define BOOL_TYPE 0x6
#define OBJECT_TYPE 0x7

#define NONE_CLASS NONE_TYPE
#define BYTE_CLASS BYTE_TYPE
#define SHORT_CLASS SHORT_TYPE
#define INT_CLASS INT_TYPE
#define LONG_CLASS LONG_TYPE
#define CHAR_CLASS CHAR_TYPE
#define BOOL_CLASS BOOL_TYPE
#define OBJECT_CLASS OBJECT_TYPE