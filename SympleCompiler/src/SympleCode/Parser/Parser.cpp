#include "Parser.h"

#include <xhash>
#include <iostream>

#include "SympleCode/Common/Node/BinaryExpressionNode.h"
#include "SympleCode/Common/Node/ExpressionStatementNode.h"

#include "SympleCode/Common/Priority.h"

namespace Symple
{
	Parser::Parser(const char* source)
		: mLexer(source), mPosition(0), mTypes(Type::PrimitiveTypes), mDiagnostics(new Diagnostics)
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
		mDiagnostics->ReportError(Peek(), "Unexpected Token '%s' of type <%s>, Expected <%s>", std::string(Peek()->GetLex()).c_str(), Token::KindString(Peek()->GetKind()), Token::KindString(kind));
		Next();
		return new Token(kind);
	}

	const Diagnostics* Parser::GetDiagnostics() const
	{
		return mDiagnostics;
	}

	bool Parser::IsType(const Token* token)
	{
		for (const Type* type : mTypes)
			if (token->GetLex() == type->GetName())
				return true;
		return false;
	}

	const Type* Parser::GetType(const Token* token)
	{
		for (const Type* type : mTypes)
			if (token->GetLex() == type->GetName())
				return type;
		return nullptr;
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
		if (IsType(Peek()) && Peek(2)->Is(Token::Kind::OpenBracket))
			return ParseFunctionDeclaration();

		return ParseGlobalStatement();
	}

	FunctionDeclarationNode* Parser::ParseFunctionDeclaration()
	{
		const Type* type = GetType(Next());
		const Token* name = Next();
		BlockStatementNode* body = ParseBlockStatement();
		Match(Token::Kind::Semicolon);

		return new FunctionDeclarationNode(type, name, body);
	}

	StatementNode* Parser::ParseStatement()
	{
		if (Peek()->Is(Token::Kind::Return))
			return ParseReturnStatement();
		if (IsType(Peek()))
			return ParseVariableDeclaration();
		ExpressionNode* expression = ParseExpression();
		Match(Token::Kind::Semicolon);
		return new ExpressionStatementNode(expression);
	}

	BlockStatementNode* Parser::ParseBlockStatement()
	{
		const Token* open = Match(Token::Kind::OpenBracket);
		std::vector<const StatementNode*> statements;
		while (!Peek()->Is(Token::Kind::CloseBracket))
		{
			if (Peek()->Is(Token::Kind::EndOfFile))
			{
				mDiagnostics->ReportError(Next(), "Unexpected End Of File");
				break;
			}

			const Token* start = Peek();

			statements.push_back(ParseStatement());

			if (start == Peek())
				Next();
		}
		const Token* close = Match(Token::Kind::CloseBracket);

		return new BlockStatementNode(open, statements, close);
	}

	ReturnStatementNode* Parser::ParseReturnStatement()
	{
		Match(Token::Kind::Return);
		ExpressionNode* expression = ParseExpression();
		Match(Token::Kind::Semicolon);
		return new ReturnStatementNode(expression);
	}

	GlobalStatementNode* Parser::ParseGlobalStatement()
	{
		return new GlobalStatementNode(ParseStatement());
	}

	VariableDeclarationNode* Parser::ParseVariableDeclaration()
	{
		const Type* type = GetType(Next());
		const Token* name = Next();
		if (Peek()->Is(Token::Kind::Equal))
		{
			Next();
			ExpressionNode* expression = ParseExpression();
			Match(Token::Kind::Semicolon);

			return new VariableDeclarationNode(name, type, expression);
		}

		Match(Token::Kind::Semicolon);

		return new VariableDeclarationNode(name, type, new ExpressionNode);
	}

	ExpressionNode* Parser::ParseExpression()
	{
		return ParseBinaryExpression();
	}

	ExpressionNode* Parser::ParseBinaryExpression(int parentPriority)
	{
		ExpressionNode* left = ParsePrimaryExpression();

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

		return ParseNameOrCallExpression();
	}

	ExpressionNode* Parser::ParseNameOrCallExpression()
	{
		if (Peek(1)->Is(Token::Kind::OpenParenthesis))
			return ParseFunctionCallExpression();

		mDiagnostics->ReportWarning(Peek(), "Empty Expression");
		return new ExpressionNode();
	}

	FunctionCallExpressionNode* Parser::ParseFunctionCallExpression()
	{
		const Token* name = Match(Token::Kind::Identifier);
		FunctionCallArgumentsNode* arguments = ParseFunctionCallArguments();

		return new FunctionCallExpressionNode(name, arguments);
	}

	FunctionCallArgumentsNode* Parser::ParseFunctionCallArguments()
	{
		const Token* open = Match(Token::Kind::OpenParenthesis);

		std::vector<const ExpressionNode*> arguments;
		while (!Peek()->Is(Token::Kind::CloseParenthesis))
		{
			if (Peek()->Is(Token::Kind::EndOfFile))
			{
				mDiagnostics->ReportError(Next(), "Unexpected End Of File");
				break;
			}

			arguments.push_back(ParseExpression());

			if (Peek()->Is(Token::Kind::Comma))
				Next();
			else
				break;
		}

		const Token* close = Match(Token::Kind::CloseParenthesis);

		return new FunctionCallArgumentsNode(open, arguments, close);
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