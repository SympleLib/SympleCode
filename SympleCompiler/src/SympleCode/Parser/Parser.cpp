#include "Parser.h"

#include <vector>
#include <xhash>

namespace Symple
{
	Parser::Parser(const char* source)
		: mLexer(source), mToken(new Token()), mPToken(new Token())
	{}

	const Token* Parser::Next()
	{
		mPToken = mToken;
		mToken = mLexer.Next();
		return mPToken;
	}

	const Token* Parser::Match(Token::Kind kind)
	{
		if (mPToken->Is(kind))
			return Next();
		return new Token(kind);
	}

	CompilationUnitNode* Parser::ParseCompilationUnit()
	{
		const std::list<MemberNode*>& members = ParseMembers();

		return new CompilationUnitNode(members);
	}

	const std::list<MemberNode*> Parser::ParseMembers()
	{
		std::list<MemberNode*> members;

		while (!mToken->Is(Token::Kind::EndOfFile))
		{
			const Token* start = mToken;

			MemberNode* member = ParseMember();
			members.push_back(member);

			if (mToken == start)
				Next();
		}

		return members;
	}

	MemberNode* Parser::ParseMember()
	{
		return ParseGlobalStatement();
	}

	GlobalStatementNode* Parser::ParseGlobalStatement()
	{
		return new GlobalStatementNode(ParseStatement());
	}

	StatementNode* Parser::ParseStatement()
	{
		switch (std::hash<std::string_view>()(mToken->GetLex()))
		{
		default:
			return ParseExpresion();
		}
	}

	ExpressionNode* Parser::ParseExpresion()
	{
		return ParseAssignmentExpresion();
	}

	ExpressionNode* Parser::ParseAssignmentExpresion()
	{
		if (mToken->Is(Token::Kind::Identifier))
		{
			Next();
			switch (mToken->GetKind())
			{
			}
		}

		return ParseBinaryExpression();
	}

	ExpressionNode* Parser::ParseBinaryExpression(int parentPrecedense)
	{
		ExpressionNode* left = ParsePrimaryExpression();

		while (true)
		{
			int precedence = 0;
			if (precedence == -1 || precedence <= parentPrecedense)
				break;

			const Token* oqerator = Next();
			ExpressionNode* right = ParseBinaryExpression(precedence);
			left = new BinaryExpressionNode(oqerator, left, right);
		}

		return left;
	}

	ExpressionNode* Parser::ParsePrimaryExpression()
	{
		switch (mToken->GetKind())
		{
		case Token::Kind::Number:
			return ParseNumberLiteral();
		}

		return new ExpressionNode;
	}

	LiteralExpressionNode* Parser::ParseNumberLiteral()
	{
		return new LiteralExpressionNode(LiteralType::Integer, Match(Token::Kind::Number));
	}
}