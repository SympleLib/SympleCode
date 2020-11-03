#ifndef _SVM_H_
#define _SVM_H_
#include <stdlib.h>
#include <string>

typedef unsigned int sid;

class _sobject {};
typedef _sobject* sobject;

typedef signed char sbyte;
typedef short sshort;
typedef int sint;
typedef long long slong;
typedef char schar;
typedef bool sbool;

typedef std::string sstring;

#define NULL_CLASS  0x0
#define BYTE_CLASS  0x1
#define SHORT_CLASS 0x2
#define INT_CLASS   0x3
#define LONG_CLASS  0x4
#define CHAR_CLASS  0x5
#define BOOL_CLASS  0x6

sid sGetVariebleID(const sstring);
sbool sVariebleExits(const sstring);
sid sGetVariebleClass(sid);

sid sGenByteVarieble(const sstring);
sid sGenShortVarieble(const sstring);
sid sGenIntVarieble(const sstring);
sid sGenLongVarieble(const sstring);
sid sGenCharVarieble(const sstring);
sid sGenBoolVarieble(const sstring);

sid sDelByteVarieble(const sstring);
sid sDelShortVarieble(const sstring);
sid sDelIntVarieble(const sstring);
sid sDelLongVarieble(const sstring);
sid sDelCharVarieble(const sstring);
sid sDelBoolVarieble(const sstring);

sbyte sGetByteVarieble(sid);
sshort sGetShortVarieble(sid);
sint sGetIntVarieble(sid);
slong sGetLongVarieble(sid);
schar sGetCharVarieble(sid);
sbool sGetBoolVarieble(sid);

sbyte& sSetByteVarieble(sid, sbyte);
sshort& sSetShortVarieble(sid, sshort);
sint& sSetIntVarieble(sid, sint);
slong& sSetLongVarieble(sid, slong);
schar& sSetCharVarieble(sid, schar);
sbool& sSetBoolVarieble(sid, sbool);
#endif