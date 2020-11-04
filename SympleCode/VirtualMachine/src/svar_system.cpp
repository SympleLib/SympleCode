#include "svar_system.h"

#include <unordered_map>

#include "sdebug.h"

namespace svm
{
	std::unordered_map<sstring, svarid> variebleNames;
	std::unordered_map<svarid, stypeid> variebleTypes;
	std::unordered_map<svarid, sclassid> variebleClasses;

	std::unordered_map<svarid, sbyte> byteVariebles;
	std::unordered_map<svarid, sshort> shortVariebles;
	std::unordered_map<svarid, sint> intVariebles;
	std::unordered_map<svarid, slong> longVariebles;
	std::unordered_map<svarid, schar> charVariebles;
	std::unordered_map<svarid, sbool> boolVariebles;
	std::unordered_map<svarid, sobject> objectVariebles;

	svarid GetVariebleID(const sstring& name)
	{
#if S_DEBUG
		if (variebleNames.find(name) == variebleNames.end())
			SEXCEPTION("Varieble '%s' doesn't exist!", name.c_str());
#endif

		return variebleNames.at(name);
	}

	stypeid GetVariebleType(svarid id)
	{
		return variebleTypes.at(id);
	}

	sclassid GetVariebleClass(svarid id)
	{
		return variebleClasses.at(id);
	}

	svarid CreateVarieble(const sstring& name, stypeid type, sclassid clazz)
	{
#if S_DEBUG
		if (variebleNames.find(name) != variebleNames.end())
			SEXCEPTION("Varieble '%s' already exists!", name.c_str());
#endif
		svarid id = std::hash<sstring>()(name);
		variebleNames.insert({ name, id });
		variebleTypes.insert({ id, type });
		variebleClasses.insert({ id, clazz });

		switch (type)
		{
		case BYTE_TYPE:
			byteVariebles.insert({ id, sbyte() });
			break;
		case SHORT_TYPE:
			shortVariebles.insert({ id, sshort() });
			break;
		case INT_TYPE:
			intVariebles.insert({ id, sint() });
			break;
		case LONG_TYPE:
			longVariebles.insert({ id, slong() });
			break;
		case CHAR_TYPE:
			charVariebles.insert({ id, schar() });
			break;
		case BOOL_TYPE:
			boolVariebles.insert({ id, sbool() });
			break;
		case OBJECT_TYPE:
			objectVariebles.insert({ id, sobject() });
			break;
		}

		return id;
	}

	void DeleteVarieble(const sstring& name)
	{
		svarid id = variebleNames.at(name);
		variebleNames.erase(name);
		stypeid type = variebleTypes.at(id);
		variebleTypes.erase(id);
		variebleClasses.erase(id);

		switch (type)
		{
		case BYTE_TYPE:
			byteVariebles.erase(id);
			break;
		case SHORT_TYPE:
			shortVariebles.erase(id);
			break;
		case INT_TYPE:
			intVariebles.erase(id);
			break;
		case LONG_TYPE:
			longVariebles.erase(id);
			break;
		case CHAR_TYPE:
			charVariebles.erase(id);
			break;
		case BOOL_TYPE:
			boolVariebles.erase(id);
			break;
		case OBJECT_TYPE:
			objectVariebles.erase(id);
			break;
		}
	}

	#pragma region Genners
	svarid CreateByteVarieble(const sstring& name, sbyte value)
	{
#if S_DEBUG
		if (variebleNames.find(name) != variebleNames.end())
			SEXCEPTION("Varieble '%s' already exists!", name.c_str());
#endif
		svarid id = std::hash<sstring>()(name);
		variebleNames.insert({ name, id });
		variebleTypes.insert({ id, BYTE_TYPE });
		variebleClasses.insert({ id, BYTE_CLASS });
		byteVariebles.insert({ id, value });

		return id;
	}
	svarid CreateShortVarieble(const sstring& name, sshort value)
	{
#if S_DEBUG
		if (variebleNames.find(name) != variebleNames.end())
			SEXCEPTION("Varieble '%s' already exists!", name.c_str());
#endif
		svarid id = std::hash<sstring>()(name);
		variebleNames.insert({ name, id });
		variebleTypes.insert({ id, SHORT_TYPE });
		variebleClasses.insert({ id, SHORT_CLASS });
		shortVariebles.insert({ id, value });

		return id;
	}
	svarid CreateIntVarieble(const sstring& name, sint value)
	{
#if S_DEBUG
		if (variebleNames.find(name) != variebleNames.end())
			SEXCEPTION("Varieble '%s' already exists!", name.c_str());
#endif
		svarid id = std::hash<sstring>()(name);
		variebleNames.insert({ name, id });
		variebleTypes.insert({ id, INT_TYPE });
		variebleClasses.insert({ id, INT_CLASS });
		intVariebles.insert({ id, value });

		return id;
	}
	svarid CreateLongVarieble(const sstring& name, slong value)
	{
#if S_DEBUG
		if (variebleNames.find(name) != variebleNames.end())
			SEXCEPTION("Varieble '%s' already exists!", name.c_str());
#endif
		svarid id = std::hash<sstring>()(name);
		variebleNames.insert({ name, id });
		variebleTypes.insert({ id, LONG_TYPE });
		variebleClasses.insert({ id, LONG_CLASS });
		longVariebles.insert({ id, value });

		return id;
	}
	svarid CreateCharVarieble(const sstring& name, schar value)
	{
#if S_DEBUG
		if (variebleNames.find(name) != variebleNames.end())
			SEXCEPTION("Varieble '%s' already exists!", name.c_str());
#endif
		svarid id = std::hash<sstring>()(name);
		variebleNames.insert({ name, id });
		variebleTypes.insert({ id, CHAR_TYPE });
		variebleClasses.insert({ id, CHAR_CLASS });
		charVariebles.insert({ id, value });

		return id;
	}
	svarid CreateBoolVarieble(const sstring& name, sbool value)
	{
#if S_DEBUG
		if (variebleNames.find(name) != variebleNames.end())
			SEXCEPTION("Varieble '%s' already exists!", name.c_str());
#endif
		svarid id = std::hash<sstring>()(name);
		variebleNames.insert({ name, id });
		variebleTypes.insert({ id, BOOL_TYPE });
		variebleClasses.insert({ id, BOOL_CLASS });
		boolVariebles.insert({ id, value });

		return id;
	}
	svarid CreateObjectVarieble(const sstring& name, sclassid clazz, sobject value)
	{
#if S_DEBUG
		if (variebleNames.find(name) != variebleNames.end())
			SEXCEPTION("Varieble '%s' already exists!", name.c_str());
#endif
		svarid id = std::hash<sstring>()(name);
		variebleNames.insert({ name, id });
		variebleTypes.insert({ id, OBJECT_TYPE });
		variebleClasses.insert({ id, clazz });
		objectVariebles.insert({ id, value });

		return id;
	}
	#pragma endregion

	#pragma region Getters
	sbyte GetByteValue(svarid id)
	{
		SASSERT(GetVariebleType(id) == BYTE_TYPE, "Illigel type!");
		return byteVariebles.at(id);
	}
	sshort GetShortValue(svarid id)
	{
		SASSERT(GetVariebleType(id) == SHORT_TYPE, "Illigel type!");
		return shortVariebles.at(id);
	}
	sint GetIntValue(svarid id)
	{
		SASSERT(GetVariebleType(id) == INT_TYPE, "Illigel type!");
		return intVariebles.at(id);
	}
	slong GetLongValue(svarid id)
	{
		SASSERT(GetVariebleType(id) == LONG_TYPE, "Illigel type!");
		return longVariebles.at(id);
	}
	schar GetCharValue(svarid id)
	{
		SASSERT(GetVariebleType(id) == CHAR_TYPE, "Illigel type!");
		return charVariebles.at(id);
	}
	sbool GetBoolValue(svarid id)
	{
		SASSERT(GetVariebleType(id) == BOOL_TYPE, "Illigel type!");
		return boolVariebles.at(id);
	}
	sobject GetObjectValue(svarid id)
	{
		SASSERT(GetVariebleType(id) == OBJECT_TYPE, "Illigel type!");
		return objectVariebles.at(id);
	}
	#pragma endregion

	#pragma region Setters
	svarid SetByteValue(svarid id, sbyte value)
	{
		SASSERT(GetVariebleType(id) == BYTE_TYPE, "Unmatched types!");
		byteVariebles.at(id) = value;

		return id;
	}
	svarid SetShortValue(svarid id, sshort value)
	{
		SASSERT(GetVariebleType(id) == SHORT_TYPE, "Unmatched types!");
		shortVariebles.at(id) = value;

		return id;
	}
	svarid SetIntValue(svarid id, sint value)
	{
		SASSERT(GetVariebleType(id) == INT_TYPE, "Unmatched types!");
		intVariebles.at(id) = value;

		return id;
	}
	svarid SetLongValue(svarid id, slong value)
	{
		SASSERT(GetVariebleType(id) == LONG_TYPE, "Unmatched types!");
		longVariebles.at(id) = value;

		return id;
	}
	svarid SetCharValue(svarid id, schar value)
	{
		SASSERT(GetVariebleType(id) == CHAR_TYPE, "Unmatched types!");
		charVariebles.at(id) = value;

		return id;
	}
	svarid SetBoolValue(svarid id, sbool value)
	{
		SASSERT(GetVariebleType(id) == BOOL_TYPE, "Unmatched types!");
		boolVariebles.at(id) = value;

		return id;
	}
	svarid SetObjectValue(svarid id, sobject value)
	{
		SASSERT(GetVariebleType(id) == OBJECT_TYPE, "Unmatched types!");
		objectVariebles.at(id) = value;

		return id;
	}
	#pragma endregion
}