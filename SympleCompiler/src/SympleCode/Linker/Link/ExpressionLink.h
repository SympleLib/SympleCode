#pragma once

#include "SympleCode/Linker/Link/Link.h"

namespace Symple
{
	class ExpressionLink : public Link
	{
	public:
		virtual Kind GetKind() const override
		{
			return Kind::Expression;
		}
	};
}