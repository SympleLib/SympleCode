#pragma once

#include "sliterals.h"

namespace svm
{
	svarid GetVariebleID(const sstring&);
	stypeid GetVariebleType(svarid);
	sclassid GetVariebleClass(svarid);

	svarid CreateVarieble(const sstring&, stypeid, sclassid);
	void DeleteVarieble(const sstring&);

	svarid CreateByteVarieble(const sstring&, sbyte);
	svarid CreateShortVarieble(const sstring&, sshort);
	svarid CreateIntVarieble(const sstring&, sint);
	svarid CreateLongVarieble(const sstring&, slong);
	svarid CreateCharVarieble(const sstring&, schar);
	svarid CreateBoolVarieble(const sstring&, sbool);
	svarid CreateObjectVarieble(const sstring&, sclassid, sobject);

	sbyte GetByteValue(svarid);
	sshort GetShortValue(svarid);
	sint GetIntValue(svarid);
	slong GetLongValue(svarid);
	schar GetCharValue(svarid);
	sbool GetBoolValue(svarid);
	sobject GetObjectValue(svarid);

	svarid SetByteValue(svarid, sbyte);
	svarid SetShortValue(svarid, sshort);
	svarid SetIntValue(svarid, sint);
	svarid SetLongValue(svarid, slong);
	svarid SetCharValue(svarid, schar);
	svarid SetBoolValue(svarid, sbool);
	svarid SetObjectValue(svarid, sobject);
}