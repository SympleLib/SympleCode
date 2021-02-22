#include "Symple/Syntax/Token.h"

using namespace Symple;

int main()
{
	Scope<Token> tok = MakeScope<Token>(TokenKind::Identifier, "test");
	

	return !getchar();
}