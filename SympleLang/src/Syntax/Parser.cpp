#include "SympleCode/Syntax/Parser.h"

#include "SympleCode/Syntax/Facts.h"

namespace Symple::Syntax
{
	Parser::Parser(shared_ptr<Lexer> lexer)
		: mLexer(lexer), mTokens(), mPosition()
	{
		while (mTokens.empty() || !mTokens.back()->Is(Token::EndOfFile))
			mTokens.push_back(mLexer->Lex());
	}

	Parser::Parser(shared_ptr<Lexer> lexer, std::vector<shared_ptr<Token>> tokens)
		: mLexer(lexer), mTokens(tokens), mPosition()
	{
		while (mTokens.empty() || !mTokens.back()->Is(Token::EndOfFile))
			mTokens.push_back(mLexer->Lex());
	}

	Parser::Parser(std::vector<shared_ptr<Token>> tokens)
		: mLexer(), mTokens(tokens), mPosition()
	{}


	shared_ptr<Node> Parser::Parse()
	{
		return ParseExpression();
	}

	shared_ptr<ExpressionNode> Parser::ParseExpression()
	{
		return ParseBinaryExpression();
	}

	shared_ptr<ExpressionNode> Parser::ParseBinaryExpression(unsigned parentPrecedence)
	{
		shared_ptr<ExpressionNode> left = ParseLiteralExpression();

		while (true)
		{
			unsigned precedence = Facts::GetBinaryOperatorPrecedence(Peek()->GetKind());
			if (precedence == 0 || precedence <= parentPrecedence)
				break;

			shared_ptr<Token> oqerator = Next();
			shared_ptr<ExpressionNode> right = ParseBinaryExpression(precedence);
			left = make_shared<BinaryExpressionNode>(left, oqerator, right);
		}

		return left;
	}

	shared_ptr<LiteralExpressionNode> Parser::ParseLiteralExpression()
	{
		return make_shared<LiteralExpressionNode>(Next());
	}


	shared_ptr<Token> Parser::Peek(unsigned off)
	{
		unsigned pos = mPosition + off;
		if (pos >= mTokens.size())
			return mTokens.back();
		return mTokens[pos];
	}

	shared_ptr<Token> Parser::Next()
	{
		auto current = Peek();
		mPosition++;
		return current;
	}
}