#include "Parser.h"

#include <vector>
#include <xhash>

#include "SympleCode/Common/Operator.h"

#include "SympleCode/Common/Node/ExpressionNode.h"
#include "SympleCode/Common/Node/BinaryExpressionNode.h"
#include "SympleCode/Common/Node/LiteralExpressionNode.h"
#include "SympleCode/Common/Node/CompilationUnitNode.h"
#include "SympleCode/Common/Node/GlobalStatementNode.h"

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
	}

	Node* Parser::ParseCompilationUnit()
	{
		const std::list<Node*>& members = ParseMembers();

		return new CompilationUnitNode(members);
	}

	const std::list<Node*> Parser::ParseMembers()
	{
		std::list<Node*> members;

		while (!mToken->Is(Token::Kind::EndOfFile))
		{
			const Token* start = mToken;

			Node* member = ParseMember();
			members.push_back(member);

			if (mToken == start)
				Next();
		}

		return members;
	}

	Node* Parser::ParseMember()
	{
		return ParseGlobalStatement();
	}

	Node* Parser::ParseGlobalStatement()
	{
		return new GlobalStatementNode(dynamic_cast<StatementNode*>(ParseStatement()));
	}

	Node* Parser::ParseStatement()
	{
		switch (std::hash<std::string_view>()(mToken->GetLex()))
		{
		default:
			return ParseExpresion();
		}
	}

	Node* Parser::ParseExpresion()
	{
		return ParseAssignmentExpresion();
	}

	Node* Parser::ParseAssignmentExpresion()
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

	Node* Parser::ParseBinaryExpression(int parentPrecedense)
	{
		ExpressionNode* left = dynamic_cast<ExpressionNode*>(ParsePrimaryExpression());

		while (true)
		{
			int precedence = 0;
			if (precedence == -1 || precedence <= parentPrecedense)
				break;

			Operator oqerator = Operator::Addition;
			ExpressionNode* right = dynamic_cast<ExpressionNode*>(ParseBinaryExpression(precedence));
			left = new BinaryExpressionNode(oqerator, left, right);
		}

		return left;
	}

	Node* Parser::ParsePrimaryExpression()
	{
		switch (mToken->GetKind())
		{
		case Token::Kind::Number:
			return ParseNumberLiteral();
		}
	}

	Node* Parser::ParseNumberLiteral()
	{
		return new LiteralExpressionNode(LiteralType::Integer, Match(Token::Kind::Number));
	}
}