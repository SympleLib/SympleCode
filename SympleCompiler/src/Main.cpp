#include <iostream>
#include "Symple/Syntax/Lexer.h"
#include "Symple/Syntax/Parser.h"

using namespace Symple;

int main()
{
	GlobalRef<File> src = MakeGlobalRef<File>("sy/Main.sy", FilePermissions::Read);
	std::cout << "Source:\n" << src->Source << '\n';

	Lexer lexer(src);
	std::vector<GlobalRef<Token>> toks;
	std::cout << "Tokens:\n";
	while (auto tok = lexer.Lex())
	{
		std::cout << TokenKindNames[(uint32)tok->Kind] << "Token '" << tok->Text << "'\n";
		if (!tok->Is(TokenKind::SingleLineComment, TokenKind::MultiLineComment))
			toks.push_back(tok);

		if (tok->Is(TokenKind::EndOfFile))
			break;
	}

	Parser parser;
	auto ast = parser.Parse(toks);
	
	return !getchar();
}