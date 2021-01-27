#include "SympleCode/Syntax/Parser.h"

#include <sstream>

#include <spdlog/spdlog.h>

#include "SympleCode/Syntax/Facts.h"

namespace Symple::Syntax
{
	Parser::Parser(shared_ptr<Lexer> lexer)
		: mLexer(lexer)
	{
		do
			mTokens.push_back(mLexer->Lex());
		while (!mTokens.back()->Is(Token::EndOfFile));
	}

	Parser::Parser(shared_ptr<Lexer> lexer, std::vector<shared_ptr<Token>> tokens)
		: mLexer(lexer), mTokens(tokens)
	{
		if (tokens.back()->Is(Token::EndOfFile))
			return;

		do
			mTokens.push_back(mLexer->Lex());
		while (!mTokens.back()->Is(Token::EndOfFile));
	}

	Parser::Parser(std::vector<shared_ptr<Token>> tokens)
		: mLexer(), mTokens(tokens)
	{}


	shared_ptr<Node> Parser::Parse()
	{
		return ParseExpression();
	}

	shared_ptr<ExpressionNode> Parser::ParseExpression()
	{
		return ParseBinaryExpression();
	}

	shared_ptr<ExpressionNode> Parser::ParseUnaryExpression(unsigned parentPrecedence)
	{
		unsigned precedence = Facts::GetUnaryOperatorPrecedence(Peek()->GetKind());
		if (precedence && precedence >= parentPrecedence)
		{
			shared_ptr<Token> oqerator = Next();
			shared_ptr<ExpressionNode> operand = ParseBinaryExpression(precedence);
			return make_shared<UnaryExpressionNode>(oqerator, operand);
		}
		else
			return ParsePrimaryExpression();
	}

	shared_ptr<ExpressionNode> Parser::ParseBinaryExpression(unsigned parentPrecedence)
	{
		shared_ptr<ExpressionNode> left = ParseUnaryExpression(parentPrecedence);

		while (true)
		{
			if (Peek()->Is(Token::EndOfFile))
			{
				return left;
			}

			unsigned precedence = Facts::GetBinaryOperatorPrecedence(Peek()->GetKind());
			if (!precedence || precedence <= parentPrecedence)
				return left;

			shared_ptr<Token> oqerator = Next();
			shared_ptr<ExpressionNode> right = ParseBinaryExpression(precedence);
			left = make_shared<BinaryExpressionNode>(oqerator, left, right);
		}
	}


	shared_ptr<ExpressionNode> Parser::ParsePrimaryExpression()
	{
		switch (Peek()->GetKind())
		{
		case Token::Number:
			return ParseLiteralExpression();
		case Token::OpenParenthesis:
			return ParseParenthesizedExpression();

		default:
			return make_shared<ExpressionNode>(Next());
		}
	}

	shared_ptr<LiteralExpressionNode> Parser::ParseLiteralExpression()
	{
		return make_shared<LiteralExpressionNode>(Next());
	}

	shared_ptr<ParenthesizedExpressionNode> Parser::ParseParenthesizedExpression()
	{
		shared_ptr<Token> open = Match(Token::OpenParenthesis);
		shared_ptr<ExpressionNode> expression = ParseExpression();
		shared_ptr<Token> close = Match(Token::CloseParenthesis);

		return make_shared<ParenthesizedExpressionNode>(open, expression, close);
	}

	
	shared_ptr<DiagnosticBag> Parser::GetDiagnosticBag()
	{ return mDiagnosticBag; }


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

	shared_ptr<Token> Parser::Match(Token::Kind kind)
	{
		if (Peek()->Is(kind))
			return Next();

		mDiagnosticBag->ReportUnexpectedToken(Peek(), kind);
		return Peek();
	}
}