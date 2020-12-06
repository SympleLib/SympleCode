#pragma once

#include <string_view>
#include <map>

#include "SympleCode/Tree/Tree.hpp"
#include "SympleCode/Type.hpp"
#include "SympleCode/Token.hpp"

#define PARSE_PRINT_LEX false
#define PARSE_PRINT_START false

namespace Symple::Parser
{
	extern std::vector<Varieble> sVars;
	extern std::map<std::string, Type> sTypes;

	Branch Parse(std::string& source);

	Branch ParseMembers();
	Branch ParseMember();

	Branch ParseFuncDecl();

	Branch ParseParams();
	Branch ParseParam();

	Branch ParseParams(const Function& func);
	Branch ParseParam(const Function& func, size_t nparam);

	Branch ParseStatement();
	Branch ParseBlock();
	Branch ParseReturn();
	Branch ParseExpr();

	Branch ParseVarDecl();
	Branch ParseBinExpr(int8_t parentOOO = -1);
	Branch ParseAssignExpr();
	Branch ParsePrimaryExpr();
	Branch ParseCondition();
	Branch ParseIfStatement();

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