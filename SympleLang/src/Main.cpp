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
using std::make_shared;

using spdlog::level::level_enum;

int main()
{
	spdlog::set_pattern("[Symple]%^<%l>%$: %v");
	spdlog::set_level(level_enum::trace);
	
	shared_ptr<Lexer> lexer = make_shared<Lexer>((char*)"sy/Main.sy");
	//shared_ptr<Token> tok = make_shared<Token>();
	//while (!tok->Is(Token::EndOfFile))
	//	(tok = lexer.Lex())->Print();

	shared_ptr<Parser> parser = make_shared<Parser>(lexer);
	shared_ptr<Node> node = parser->Parse();
	node->Print();

	return !getc(stdin);
}