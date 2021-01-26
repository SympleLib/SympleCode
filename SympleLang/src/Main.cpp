#include <iostream>
#include <cstdlib>

#include <spdlog/spdlog.h>

#include "SympleCode/Syntax/Lexer.h"

using Symple::Lexer;
using Symple::Token;

using spdlog::level::level_enum;

int main()
{
	spdlog::set_pattern("[Symple]%^<%l>%$: %v");
	spdlog::set_level(level_enum::trace);
	
	Lexer lexer("sy/Main.sy");
	std::shared_ptr<Token> tok = std::make_shared<Token>();
	while (!tok->Is(Token::EndOfFile))
		(tok = lexer.Lex())->Print();

	return !getc(stdin);
}