#pragma once

#include <vector>

#include "SympleCode/Syntax/Token.h"

namespace Symple
{
	class Parser
	{
	private:
		std::vector<std::shared_ptr<Token>> mTokens;
		unsigned mPosision;
	public:
		Parser(char* file);
		Parser(std::vector<std::shared_ptr<Token>>& tokens);
	};
}