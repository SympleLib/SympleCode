#pragma once

#include "SympleCode/Common/Token.h"

namespace Symple
{
	class Priority
	{
	private:
		Priority();
	public:
		static int BinaryOperatorPriority(const Token* token);
	};
}