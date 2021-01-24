#include <iostream>
#include <cstdlib>

#include "SympleCode/Syntax/Lexer.h"

using Symple::Lexer;
using Symple::Token;

int main()
{
	Lexer lexer("sy/Main.sy");
	std::shared_ptr<Token> tok = std::make_shared<Token>();
	while (!tok->Is(Token::EndOfFile))
	{
		tok = lexer.Lex();
		std::cout << '[' << Token::KindMap[tok->GetKind()] << "] " << tok->GetText() << std::endl;
	}

	getc(stdin);
}