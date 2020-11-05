#pragma once

#include <vector>
#include <unordered_map>

#include <sliterals.h>

namespace scm
{
	void InitPrimitiveClasses();
	void CreateClass(const sstring&, stypeid, const std::vector<sclassid>&, ssize);

	const std::unordered_map<sstring, sclassid>& GetClassNames();
	sclassid GetClassID(const sstring&);
	stypeid GetClassType(sclassid);
	const std::vector<sclassid>& GetClassSupers(sclassid);
}