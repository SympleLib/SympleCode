#include <iostream>
#include <cstdlib>

#include <spdlog/spdlog.h>

#include "SympleCode/Syntax/Lexer.h"
#include "SympleCode/Syntax/Parser.h"

using Symple::Lexer;
using Symple::Token;
using Symple::Parser;

using Symple::Node;

using std::shared_ptr;

using spdlog::level::level_enum;

int main()
{
	spdlog::set_pattern("[Symple]%^<%l>%$: %v");
	spdlog::set_level(level_enum::trace);
	
	shared_ptr<Lexer> lexer = std::make_shared<Lexer>((char*)"sy/Main.sy");
	//std::shared_ptr<Token> tok = std::make_shared<Token>();
	//while (!tok->Is(Token::EndOfFile))
	//	(tok = lexer.Lex())->Print();

	shared_ptr<Parser> parser = std::make_shared<Parser>(lexer);
	shared_ptr<Node> node = parser->Parse();
	node->Print();

	return !getc(stdin);
}