#include "SympleCode/Common/Analysis/Type.h"

namespace Symple
{
	const std::vector<const Type*> Type::PrimitiveTypes = {
		new Type("void", 0),
		new Type("byte", 1),
		new Type("short", 2),
		new Type("int", 4),
		new Type("long", 8),

		new Type("bool", 1),
		new Type("char", 1),
		new Type("wchar", 2),
	};
}