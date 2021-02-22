#include <iostream>
#include "Symple/Syntax/Lexer.h"
#include "Symple/Syntax/Parser.h"

using namespace Symple;

int main()
{
	Lexer lexer(MakeGlobalRef<File>("sy/Main.sy", FilePermissions::Read));
	std::vector<GlobalRef<Token>> toks;
	while (auto tok = lexer.Lex())
	{
		std::cout << TokenKindNames[(uint32)tok->Kind] << "Token '" << tok->Text << "'\n";
		toks.push_back(tok);

		if (tok->Kind == TokenKind::EndOfFile)
			break;
	}

	Parser parser;
	auto ast = parser.Parse(toks);
	
	return !getchar();
}