#include "Parser.h"

#include <xhash>
#include <iostream>

#include "SympleCode/Common/Node/BinaryExpressionNode.h"

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

	CompilationUnitNode* Parser::ParseCompilationUnit()
	{
		std::vector<const Node*> expressions;
		while (!Peek()->Is(Token::Kind::EndOfFile))
		{
			expressions.push_back(ParseBinaryExpression());
		}
		return new CompilationUnitNode(expressions);
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
		if (Peek()->Is(Token::Kind::Semicolon))
		{
			Next();
			return left;
		}

		while (!Peek()->Is(Token::Kind::EndOfFile))
		{
			int priority = Priority::BinaryOperatorPriority(Peek());
			if (priority < 0 || priority <= parentPriority)
				break;
			const Token* oqerator = Next();
			ExpressionNode* right = ParseBinaryExpression(priority);
			left = new BinaryExpressonNode(oqerator, left, right);
		}

		return left;
	}

	ExpressionNode* Parser::ParsePrimaryExpression()
	{
		switch (Peek()->GetKind())
		{
		case Token::Kind::True:
		case Token::Kind::False:
			return ParseBooleanLiteral();
		case Token::Kind::Number:
			return ParseNumberLiteral();
		}

		return new LiteralExpressionNode(Next());
	}

	NumberLiteralExpressionNode* Parser::ParseNumberLiteral()
	{
		return new NumberLiteralExpressionNode(Match(Token::Kind::Number));
	}

	BooleanLiteralExpressionNode* Parser::ParseBooleanLiteral()
	{
		return new BooleanLiteralExpressionNode(Next());
	}
}