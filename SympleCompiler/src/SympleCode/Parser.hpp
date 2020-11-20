#pragma once

#include <string_view>

#include "SympleCode/Tree/Tree.hpp"
#include "SympleCode/Type.hpp"
#include "SympleCode/Token.hpp"

namespace Symple::Parser
{
	void Parse(std::string& source);

	Branch ParseMembers();
	Branch ParseMember();

	Branch ParseFuncDecl();

	Branch ParseParams();
	Branch ParseParam();

	Branch ParseStatement();
	Branch ParseBlock();
	Branch ParseExpr();

	Branch ParseSetExpr();
	Branch ParseBinExpr(int8_t parentOOO = -1);
	Branch ParsePrimaryExpr();

	Type ParseType();

	int8_t GetUnOpOOO(Token tok);
	int8_t GetBinOpOOO(Token tok);
	int8_t GetLogOpOOO(Token tok);

	TokenInfo Peek(size_t offset = 0);
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