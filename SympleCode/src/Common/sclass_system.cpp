#include "sclass_system.h"

#include <sdebug.h>

namespace scm
{
	std::unordered_map<sstring, sclassid> classNames;
	std::unordered_map<sclassid, stypeid> classTypes;
	std::unordered_map<sclassid, std::vector<sclassid>> classSupers;
	std::unordered_map<sclassid, ssize> classSize;

	void InitPrimitiveClasses()
	{
		CreateClass("byte", BYTE_TYPE, {}, sizeof(sbyte));
		CreateClass("short", SHORT_TYPE, {}, sizeof(sshort));
		CreateClass("int", INT_TYPE, {}, sizeof(sint));
		CreateClass("long", LONG_TYPE, {}, sizeof(slong));
		CreateClass("char", CHAR_TYPE, {}, sizeof(schar));
		CreateClass("bool", BOOL_TYPE, {}, sizeof(sbool));

		CreateClass("ubyte", BYTE_TYPE, {}, sizeof(subyte));
		CreateClass("ushort", SHORT_TYPE, {}, sizeof(sushort));
		CreateClass("uint", INT_TYPE, {}, sizeof(suint));
		CreateClass("ulong", LONG_TYPE, {}, sizeof(sulong));

		CreateClass("object", OBJECT_TYPE, {}, sizeof(void*));
	}

	void CreateClass(const sstring& name, stypeid type, const std::vector<sclassid>& supers, ssize size)
	{
#if S_DEBUG
		if (classNames.find(name) != classNames.end())
			SERROR("Class '%s' already exist!", name.c_str());
#endif
		sclassid id = std::hash<sstring>()(name);
		classNames.insert({ name, id });
		classTypes.insert({ id, type });
		classSupers.insert({ id, supers });
		classSize.insert({ id, size });
	}

	const std::unordered_map<sstring, sclassid>& GetClassNames()
	{
		return classNames;
	}

	sclassid GetClassID(const sstring& name)
	{
		return classNames.at(name);
	}

	stypeid GetClassType(sclassid id)
	{
		return classTypes.at(id);
	}

	const std::vector<stypeid>& GetClassSupers(sclassid id)
	{
		return classSupers.at(id);
	}
}