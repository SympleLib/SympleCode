#include "SympleCode/Syntax/Parser.h"

#include <spdlog/spdlog.h>

#include "SympleCode/Syntax/Lexer.h"

namespace Symple
{
	Parser::Parser(char* file)
		: mPosition()
	{
		Lexer lexer(file);
		std::shared_ptr<Token> tok = std::make_shared<Token>();
		while (!tok->Is(Token::EndOfFile))
			mTokens.push_back(tok = lexer.Lex());
	}

	Parser::Parser(std::vector<std::shared_ptr<Token>>& tokens)
		: mPosition(), mTokens(tokens)
	{}


	std::shared_ptr<Token> Parser::Peek(unsigned off)
	{
		unsigned pos = mPosition + off;
		if (pos >= mTokens.size())
			return mTokens.back();
		return mTokens[pos];
	}

	std::shared_ptr<Token> Parser::Next()
	{
		std::shared_ptr<Token> current = Peek();
		mPosition++;
		return current;
	}

	std::shared_ptr<Token> Parser::Match(Token::Kind kind)
	{
		if (!Peek()->Is(kind))
			spdlog::error("Unexpected {} '{}', Expected {}", Token::KindMap[Peek()->GetKind()], Peek()->GetText(), Token::KindMap[kind]);
		return Next();
	}


	std::shared_ptr<ExpressionNode> Parser::ParseExpression()
	{
		return ParsePrimaryExpression();
	}

	std::shared_ptr<ExpressionNode> Parser::ParsePrimaryExpression()
	{
		switch (Peek()->GetKind())
		{
		case Token::Number:
			return ParseLiteralExpression();
		}

		spdlog::debug("Token: {}", Next()->GetText());
		return std::make_shared<ExpressionNode>(std::make_shared<Token>());
	}

	std::shared_ptr<LiteralExpressionNode> Parser::ParseLiteralExpression()
	{
		spdlog::debug("Token: {}", Peek()->GetText());
		return std::make_shared<LiteralExpressionNode>(Next());
	}
}