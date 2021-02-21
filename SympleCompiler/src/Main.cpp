#include <cstdio>

#include "Symple/Syntax/Token.h"

using namespace Symple;

int main()
{
	Scope<Token> tok = MakeScope<Token>(TokenKind::EndOfFile);
	puts(TokenKindNames[(int)tok->Kind]);

	return !getchar();
}