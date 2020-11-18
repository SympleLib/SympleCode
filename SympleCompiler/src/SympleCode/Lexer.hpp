#pragma once

#include <string>
#include <functional>

#include "SympleCode/Token.hpp"

namespace Lexer
{
	bool IsDigit(char);
	bool IsIdentifier(char);
	bool IsSpace(char);

	char Peek();
	char Get();

	void Lex(const char*, std::function<bool(const TokenInfo&)>);
	TokenInfo Next();

	TokenInfo Atom(Token);
	TokenInfo Identifier();
	TokenInfo Number();
	TokenInfo Comment();
}