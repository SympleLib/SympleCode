#pragma once

#include <string_view>

#include "SympleCode/Tree/Tree.hpp"
#include "SympleCode/Token.hpp"

namespace Symple::Parser
{
	void Parse(std::string& source);

	Branch ParseExpr(int8_t parentOOO = -1);
	Branch ParsePrimaryExpr();

	int8_t GetUnOpOOO(Token tok);
	int8_t GetBinOpOOO(Token tok);

	TokenInfo Peek(size_t offset);
	TokenInfo Next();
	TokenInfo PNext();

	TokenInfo Match(Token tok);

	//Branch& ParseExpresion(const Type& type, const Token* beg, const Token* end);
#ifdef _WIN64
	int64_t ParseInt(std::string_view view);
#else
	int32_t ParseInt(std::string_view view);
#endif
}