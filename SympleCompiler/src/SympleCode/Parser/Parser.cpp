#include "Parser.h"

#include <xhash>
#include <iostream>

#include "SympleCode/Common/Node/BinaryExpressionNode.h"
#include "SympleCode/Common/Node/ExpressionStatementNode.h"

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
		auto members = ParseMembers();
		Match(Token::Kind::EndOfFile);
		return new CompilationUnitNode(members);
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

	const std::vector<const MemberNode*> Parser::ParseMembers()
	{
		std::vector<const MemberNode*> members;
		while (!Peek()->Is(Token::Kind::EndOfFile))
		{
			const Token* start = Peek();
			members.push_back(ParseMember());

			if (start == Peek())
				Next();
		}

		return members;
	}

	MemberNode* Parser::ParseMember()
	{
		if (Peek()->Is(Token::Kind::Function))
			return ParseFunctionDeclaration();

		return ParseGlobalStatement();
	}

	FunctionDeclarationNode* Parser::ParseFunctionDeclaration()
	{
		Match(Token::Kind::Function);
		const Token* type = Next();
		const Token* name = Next();
		BlockStatementNode* body = ParseBlockStatement();
		Match(Token::Kind::Semicolon);

		return new FunctionDeclarationNode(type, name, body);
	}

	GlobalStatementNode* Parser::ParseGlobalStatement()
	{
		return new GlobalStatementNode(ParseStatement());
	}

	BlockStatementNode* Parser::ParseBlockStatement()
	{
		const Token* open = Match(Token::Kind::OpenBracket);
		std::vector<const StatementNode*> statements;
		while (!Peek()->Is(Token::Kind::CloseBracket))
		{
			const Token* start = Peek();

			statements.push_back(ParseStatement());

			if (start == Peek())
				Next();
		}
		const Token* close = Match(Token::Kind::CloseBracket);

		return new BlockStatementNode(open, statements, close);
	}

	StatementNode* Parser::ParseStatement()
	{
		return new ExpressionStatementNode(ParseExpression());
	}

	ExpressionNode* Parser::ParseExpression()
	{
		return ParseBinaryExpression();
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
			left = new BinaryExpressionNode(oqerator, left, right);
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