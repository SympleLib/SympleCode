#ifndef _SPARSER_H_
#define _SPARSER_H_
#include "svm.h"

void sParseFile(const sstring&);
void sParseSource(const sstring&);
void sPreParseLine(sstring&);
void sParseLine(const sstring&);

sbyte sParseByte(const sstring&);
sshort sParseShort(const sstring&);
sint sParseInt(const sstring&);
slong sParseLong(const sstring&);
schar sParseChar(const sstring&);
sbool sParseBool(const sstring&);

unsigned int sGetCurrentLine();
std::string sGetCurrentFile();
#endif