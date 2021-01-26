﻿#include <iostream>
#include <cstdlib>

#include <spdlog/spdlog.h>

#include "SympleCode/Syntax/Lexer.h"
#include "SympleCode/Syntax/Parser.h"

using Symple::Syntax::Lexer;
using Symple::Syntax::Token;
using Symple::Syntax::Parser;

using Symple::Syntax::ExpressionNode;

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
	shared_ptr<ExpressionNode> node = parser->ParseExpression();
	node->Print();
	putchar('\n');

	if (node->CanEvaluate())
		printf("Value = %i\n", node->Evaluate());

	return !getc(stdin);
}