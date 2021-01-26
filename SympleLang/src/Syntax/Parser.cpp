#include "SympleCode/Syntax/Parser.h"

namespace Symple
{
	Parser::Parser(std::shared_ptr<Lexer> lexer)
		: mLexer(lexer), mTokens(), mPosision()
	{
		while (!mTokens.back()->Is(Token::EndOfFile))
			mTokens.push_back(mLexer->Lex());
	}

	Parser::Parser(std::shared_ptr<Lexer> lexer, std::vector<std::shared_ptr<Token>> tokens)
		: mLexer(lexer), mTokens(tokens), mPosision()
	{
		while (!mTokens.back()->Is(Token::EndOfFile))
			mTokens.push_back(mLexer->Lex());
	}

	Parser::Parser(std::vector<std::shared_ptr<Token>> tokens)
		: mLexer(), mTokens(tokens), mPosision()
	{}


	std::shared_ptr<Node> Parser::Parse()
	{
		return ParseExpression();
	}

	std::shared_ptr<ExpressionNode> Parser::ParseExpression()
	{

	}
}