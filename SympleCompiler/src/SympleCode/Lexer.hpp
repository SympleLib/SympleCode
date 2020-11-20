#pragma once

#include <string>
#include <functional>

#include "SympleCode/Token.hpp"

namespace Symple::Lexer
{
	bool IsDigit(char c);
	bool IsIdentifier(char c);
	bool IsSpace(char c);

	char Peek();
	char Get();

	void Lex(const char* beg, std::function<bool(const TokenInfo&)> func);
	TokenInfo Next();

	TokenInfo Atom(Token);
	TokenInfo Identifier();
	TokenInfo Number();
	TokenInfo Comment();
	TokenInfo Equals();
}