#include "SympleCode/Syntax/Parser.h"

#include <spdlog/spdlog.h>

namespace Symple
{
	Parser::Parser(char* file)
		: mPosition(), mLexer(std::make_unique<Lexer>(file))
	{
		std::shared_ptr<Token> tok = std::make_shared<Token>();
		while (!tok->Is(Token::EndOfFile))
			mTokens.push_back(tok = mLexer->Lex());

		putchar('\n');
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
		current->Print();

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
		
		return std::make_shared<ExpressionNode>(Next());
	}

	std::shared_ptr<LiteralExpressionNode> Parser::ParseLiteralExpression()
	{
		return std::make_shared<LiteralExpressionNode>(Next());
	}
}