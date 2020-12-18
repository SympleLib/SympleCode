#pragma once

#include "SympleCode/Common/Token.h"

namespace Symple
{
	class Priority
	{
	private:
		Priority();
	public:
		static int UnaryOperatorPriority(const Token* token);
		static int BinaryOperatorPriority(const Token* token);
	};
}