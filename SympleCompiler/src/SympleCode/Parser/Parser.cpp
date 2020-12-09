#include "Parser.h"

#include <xhash>
#include <iostream>

#include "SympleCode/Common/Node/BinaryExpressionNode.h"
#include "SympleCode/Common/Node/LiteralExpressionNode.h"

#include "SympleCode/Common/Priority.h"

namespace Symple
{
	Parser::Parser(const char* source)
		: mLexer(source), mPosition(0)
	{
		Token* current = new Token;
		while (!current->Is(Token::Kind::EndOfFile))
		{
			current = mLexer.Next();
			mTokens.push_back(current);
		}
	}

	const Token* Parser::Peek(size_t offset)
	{
		size_t i = mPosition + offset;
		if (i >= mTokens.size())
			return mTokens.back();
		return mTokens[i];
	}

	const Token* Parser::Next()
	{
		const Token* current = Peek();
		mPosition++;
		return current;
	}

	const Token* Parser::Match(Token::Kind kind)
	{
		if (Peek()->Is(kind))
			return Next();
		return new Token(kind);
	}

	ExpressionNode* Parser::ParseBinaryExpression(int parentPriority)
	{
		ExpressionNode* left = ParsePrimaryExpression();

		while (true)
		{
			const Token* oqerator = Next();
			int priority = Priority::BinaryOperatorPriority(oqerator);
			if (priority == -1 || priority <= parentPriority)
				break;
			ExpressionNode* right = ParseBinaryExpression(priority);
			left = new BinaryExpressonNode(oqerator, left, right);
		}

		return left;
	}

	ExpressionNode* Parser::ParsePrimaryExpression()
	{
		return new LiteralExpressionNode(Match(Token::Kind::Number));
	}
}