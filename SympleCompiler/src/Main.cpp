#include <iostream>
#include "Symple/Syntax/Lexer.h"

using namespace Symple;

int main()
{
	Lexer lexer(MakeGlobalRef<File>("sy/Main.sy", FilePermissions::Read));
	while (auto tok = lexer.Lex())
	{
		std::cout << TokenKindNames[(int32)tok->Kind] << "Token '" << tok->Text << "'\n";

		if (tok->Kind == TokenKind::EndOfFile)
			break;
	}
	
	return !getchar();
}