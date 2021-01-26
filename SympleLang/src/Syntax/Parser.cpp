#include "SympleCode/Syntax/Parser.h"

namespace Symple
{
	Parser::Parser(std::shared_ptr<Lexer> lexer)
		: mLexer(lexer), mTokens(), mPosition()
	{
		while (mTokens.empty() || !mTokens.back()->Is(Token::EndOfFile))
			mTokens.push_back(mLexer->Lex());
	}

	Parser::Parser(std::shared_ptr<Lexer> lexer, std::vector<std::shared_ptr<Token>> tokens)
		: mLexer(lexer), mTokens(tokens), mPosition()
	{
		while (mTokens.empty() || !mTokens.back()->Is(Token::EndOfFile))
			mTokens.push_back(mLexer->Lex());
	}

	Parser::Parser(std::vector<std::shared_ptr<Token>> tokens)
		: mLexer(), mTokens(tokens), mPosition()
	{}


	std::shared_ptr<Node> Parser::Parse()
	{
		return ParseExpression();
	}

	std::shared_ptr<ExpressionNode> Parser::ParseExpression()
	{
		return ParseLiteralExpression();
	}

	std::shared_ptr<LiteralExpressionNode> Parser::ParseLiteralExpression()
	{
		return std::make_shared<LiteralExpressionNode>(Next());
	}


	std::shared_ptr<Token> Parser::Peek(unsigned off)
	{
		unsigned pos = mPosition + off;
		if (pos >= mTokens.size())
			return mTokens.back();
		return mTokens[pos];
	}

	std::shared_ptr<Token> Parser::Next()
	{
		auto current = Peek();
		mPosition++;
		return current;
	}
}