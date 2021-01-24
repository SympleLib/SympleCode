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

	Lexer lexer("sy/Main.sy");
	std::shared_ptr<Token> tok = std::make_shared<Token>();
	while (!tok->Is(Token::EndOfFile))
	{
		tok = lexer.Lex();
		spdlog::info("[{}] {}", Token::KindMap[tok->GetKind()], tok->GetText());
	}

	Parser parser("sy/Main.sy");
	std::shared_ptr<ExpressionNode> expr = parser.ParseExpression();
	spdlog::info("Expr = {{ Type: {}, Token: {{ Type: {}, Text: {} }} }}", Node::KindMap[expr->GetKind()], Token::KindMap[expr->GetToken()->GetKind()], expr->GetToken()->GetText());

	return !getc(stdin);
}