#include <iostream>
#include <cstdlib>

#include <spdlog/spdlog.h>

#include "SympleCode/Syntax/Lexer.h"
#include "SympleCode/Syntax/Parser.h"

using Symple::ExpressionNode;
using Symple::Node;
using Symple::Parser;
using Symple::Lexer;
using Symple::Token;

using spdlog::level::level_enum;

int main()
{
	spdlog::set_pattern("[Symple]%^<%l>%$: %v");
	spdlog::set_level(level_enum::trace);
	
	Parser parser("sy/Main.sy");
	std::shared_ptr<ExpressionNode> expr = parser.ParseExpression();

	return !getc(stdin);
}