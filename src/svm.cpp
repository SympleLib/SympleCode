#include "svm.h"

#include <unordered_map>

#include "sparser.h"
#include "sstdlib/sdebug.h"

std::unordered_map<std::string, sid> variebleNames;
std::unordered_map<sid, sid> variebleTypes;
std::unordered_map<sid, sobject> objectVariebles;
std::unordered_map<sid, sbyte> byteVariebles;
std::unordered_map<sid, sshort> shortVariebles;
std::unordered_map<sid, sint> intVariebles;
std::unordered_map<sid, slong> longVariebles;
std::unordered_map<sid, schar> charVariebles;
std::unordered_map<sid, sbool> boolVariebles;

sid sCreateVarieble(const sstring name)
{
	if (sVariebleExits(name))
		SERRORF("Duplicate Varieble: '%s'!", name.c_str());
	sid id = std::hash<std::string>()(name);
	variebleNames.insert({ name, id });
	return id;
}

sid sGetVariebleID(const sstring name)
{
	if (sVariebleExits(name))
		return variebleNames.at(name);
	else
	{
		SERRORF("Varieble '%s' doesn't exist!", name.c_str());
	}
}

sbool sVariebleExits(const sstring name)
{
	return variebleNames.find(name) != variebleNames.end();
}

sid sGetVariebleClass(sid var)
{
	if (variebleTypes.find(var) != variebleTypes.end())
		return variebleTypes.at(var);
	else
		return NULL_CLASS;
}

#pragma region Genners
sid sGenByteVarieble(const sstring name)
{
	sid id = sCreateVarieble(name);
	byteVariebles.insert({ id, (sbyte)0 });
	variebleTypes.insert({ id, BYTE_CLASS });
	return id;
}

sid sGenShortVarieble(const sstring name)
{
	sid id = sCreateVarieble(name);
	shortVariebles.insert({ id, (sshort)0 });
	variebleTypes.insert({ id, SHORT_CLASS });
	return id;
}

sid sGenIntVarieble(const sstring name)
{
	sid id = sCreateVarieble(name);
	intVariebles.insert({ id, (sint)0 });
	variebleTypes.insert({ id, INT_CLASS });
	return id;
}

sid sGenLongVarieble(const sstring name)
{
	sid id = sCreateVarieble(name);
	longVariebles.insert({ id, (slong)0 });
	variebleTypes.insert({ id, LONG_CLASS });
	return id;
}

sid sGenCharVarieble(const sstring name)
{
	sid id = sCreateVarieble(name);
	charVariebles.insert({ id, (schar)0 });
	variebleTypes.insert({ id, CHAR_CLASS });
	return id;
}

sid sGenBoolVarieble(const sstring name)
{
	sid id = sCreateVarieble(name);
	boolVariebles.insert({ id, (sbool)0 });
	variebleTypes.insert({ id, BOOL_CLASS });
	return id;
}
#pragma endregion

#pragma region Dellers
sid sDelByteVarieble(const sstring name)
{
	sid id = sGetVariebleID(name);
	byteVariebles.erase(id);
	variebleTypes.erase(id);
	variebleNames.erase(name);
	return id;
}

sid sDelShortVarieble(const sstring name)
{
	sid id = sGetVariebleID(name);
	shortVariebles.erase(id);
	variebleTypes.erase(id);
	variebleNames.erase(name);
	return id;
}

sid sDelIntVarieble(const sstring name)
{
	sid id = sGetVariebleID(name);
	intVariebles.erase(id);
	variebleTypes.erase(id);
	variebleNames.erase(name);
	return id;
}

sid sDelLongVarieble(const sstring name)
{
	sid id = sGetVariebleID(name);
	longVariebles.erase(id);
	variebleTypes.erase(id);
	variebleNames.erase(name);
	return id;
}

sid sDelCharVarieble(const sstring name)
{
	sid id = sGetVariebleID(name);
	charVariebles.erase(id);
	variebleTypes.erase(id);
	variebleNames.erase(name);
	return id;
}

sid sDelBoolVarieble(const sstring name)
{
	sid id = sGetVariebleID(name);
	boolVariebles.erase(id);
	variebleTypes.erase(id);
	variebleNames.erase(name);
	return id;
}
#pragma endregion

#pragma region Getters
sbyte sGetByteVarieble(sid id)
{
	return byteVariebles.at(id);
}

sshort sGetShortVarieble(sid id)
{
	return shortVariebles.at(id);
}

sint sGetIntVarieble(sid id)
{
	return intVariebles.at(id);
}

slong sGetLongVarieble(sid id)
{
	return longVariebles.at(id);
}

schar sGetCharVarieble(sid id)
{
	return charVariebles.at(id);
}

sbool sGetBoolVarieble(sid id)
{
	return boolVariebles.at(id);
}
#pragma endregion

#pragma region Setters
sbyte& sSetByteVarieble(sid id, sbyte v)
{
	return byteVariebles.at(id) = v;
}

sshort& sSetShortVarieble(sid id, sshort v)
{
	return shortVariebles.at(id) = v;
}

sint& sSetIntVarieble(sid id, sint v)
{
	return intVariebles.at(id) = v;
}

slong& sSetLongVarieble(sid id, slong v)
{
	return longVariebles.at(id) = v;
}

schar& sSetCharVarieble(sid id, schar v)
{
	return charVariebles.at(id) = v;
}

sbool& sSetBoolVarieble(sid id, sbool v)
{
	return boolVariebles.at(id) = v;
}
#pragma endregion