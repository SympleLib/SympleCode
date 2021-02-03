#include "SympleCode/Syntax/Parser.h"

#include <sstream>

#include <spdlog/spdlog.h>

#include "SympleCode/Syntax/Facts.h"

namespace Symple::Syntax
{
	Parser::Parser(shared_ptr<Lexer> lexer)
	{
		do
			mTokens.push_back(lexer->Lex());
		while (!mTokens.back()->Is(Token::EndOfFile));
	}

	Parser::Parser(shared_ptr<Lexer> lexer, std::vector<shared_ptr<Token>> tokens)
		: mTokens(tokens)
	{
		if (!tokens.empty() && tokens.back()->Is(Token::EndOfFile))
			return;

		do
			mTokens.push_back(lexer->Lex());
		while (!mTokens.back()->Is(Token::EndOfFile));
	}

	Parser::Parser(std::vector<shared_ptr<Token>> tokens)
		: mTokens(tokens)
	{}


	shared_ptr<Syntax::Node> Parser::Parse()
	{ return ParseMember(); }


	shared_ptr<MemberSyntax> Parser::ParseMember()
	{
		if (IsType())
			return ParseFunctionDeclaration();
		else
			return make_shared<MemberSyntax>(Next());
	}

	shared_ptr<FunctionDeclarationSyntax> Parser::ParseFunctionDeclaration()
	{
		auto type = ParseType();
		shared_ptr<Token> name = Match(Token::Identifier);
		shared_ptr<Token> openParen = Match(Token::OpenParenthesis);
		auto params = ParseVariableDeclarationList();
		shared_ptr<Token> closeParen = Match(Token::CloseParenthesis);
		shared_ptr<StatementSyntax> statement = ParseStatement();

		return make_shared<FunctionDeclarationSyntax>(type, name, openParen, params, closeParen, statement);
	}


	shared_ptr<StatementSyntax> Parser::ParseStatement(bool matchSemi)
	{
		shared_ptr<StatementSyntax> statement;

		if (IsType())
			statement = ParseVariableDeclaration();

		switch (Peek()->GetKind())
		{
		case Token::OpenBrace:
			statement = ParseBlockStatement();
			matchSemi = false;
			break;
		case Token::ReturnKeyword:
			statement = ParseReturnStatement();
			break;

		default:
			if (!statement)
				statement = make_shared<StatementSyntax>(Peek());
			break;
		}

		if (matchSemi)
			Match(Token::Semicolon);
		return statement;
	}

	shared_ptr<BlockStatementSyntax> Parser::ParseBlockStatement()
	{
		shared_ptr<Token> open = Match(Token::OpenBrace);
		std::vector<shared_ptr<StatementSyntax>> statements;

		while (!Peek()->Is(Token::CloseBrace))
		{
			if (Peek()->Is(Token::EndOfFile))
			{
				mDiagnosticBag->ReportUnexpectedEndOfFile(Peek());
				return make_shared<BlockStatementSyntax>(open, statements, Peek());
			}

			unsigned start = mPosition;
			shared_ptr<StatementSyntax> statement = ParseStatement();
			statements.push_back(statement);

			if (mPosition == start)
				Next();
		}

		shared_ptr<Token> close = Match(Token::CloseBrace);

		return make_shared<BlockStatementSyntax>(open, statements, close);
	}

	shared_ptr<ReturnStatementSyntax> Parser::ParseReturnStatement()
	{
		shared_ptr<Token> tok = Match(Token::ReturnKeyword);
		shared_ptr<ExpressionSyntax> val = ParseExpression();

		return make_shared<ReturnStatementSyntax>(tok, val);
	}

	shared_ptr<VariableDeclarationSyntax> Parser::ParseVariableDeclaration()
	{
		shared_ptr<TypeSyntax> type = ParseType();
		shared_ptr<Token> name = Token::Default;
		if (Peek()->Is(Token::Identifier))
			name = Next();

		shared_ptr<Token> equals = Token::Default;
		shared_ptr<ExpressionSyntax> initializer;
		if (Peek()->Is(Token::Equal))
		{
			equals = Next();
			initializer = ParseExpression();
		}

		return make_shared<VariableDeclarationSyntax>(type, name, equals, initializer);
	}

	std::vector<shared_ptr<VariableDeclarationSyntax>> Parser::ParseVariableDeclarationList()
	{
		std::vector<shared_ptr<VariableDeclarationSyntax>> list;
		
		if (IsType())
			list.push_back(ParseVariableDeclaration());
		while (Peek()->Is(Token::Comma))
		{
			Next();
			list.push_back(ParseVariableDeclaration());
		}

		return list;
	}

	shared_ptr<TypeSyntax> Parser::ParseType(shared_ptr<TypeSyntax> base)
	{
		shared_ptr<Token> tyqename = Next();
		base = make_shared<TypeReferenceSyntax>(tyqename, base);
		if (IsType())
			return ParseType(base);
		else
			return base;
	}


	shared_ptr<ExpressionSyntax> Parser::ParseExpression()
	{ return ParseBinaryExpression(); }

	shared_ptr<ExpressionSyntax> Parser::ParseUnaryExpression(unsigned parentPrecedence)
	{
		unsigned precedence = Facts::GetUnaryOperatorPrecedence(Peek()->GetKind());
		if (precedence && precedence >= parentPrecedence)
		{
			shared_ptr<Token> oqerator = Next();
			shared_ptr<ExpressionSyntax> operand = ParseBinaryExpression(precedence);
			return make_shared<UnaryExpressionSyntax>(oqerator, operand);
		}
		else
			return ParsePrimaryExpression();
	}

	shared_ptr<ExpressionSyntax> Parser::ParseBinaryExpression(unsigned parentPrecedence)
	{
		shared_ptr<ExpressionSyntax> left = ParseUnaryExpression(parentPrecedence);

		while (true)
		{
			unsigned precedence = Facts::GetBinaryOperatorPrecedence(Peek()->GetKind());
			if (!precedence || precedence <= parentPrecedence)
				return left;

			shared_ptr<Token> oqerator = Next();
			shared_ptr<ExpressionSyntax> right = ParseUnaryExpression(precedence);
			left = make_shared<BinaryExpressionSyntax>(oqerator, left, right);
		}
	}


	shared_ptr<ExpressionSyntax> Parser::ParsePrimaryExpression()
	{
		switch (Peek()->GetKind())
		{
		case Token::Identifier:
			return ParseNameExpression();
		case Token::Number:
		case Token::Float:
		case Token::Integer:
			return ParseLiteralExpression();
		case Token::OpenParenthesis:
			return ParseParenthesizedExpression();

		default:
			return make_shared<ExpressionSyntax>(Next());
		}
	}

	shared_ptr<NameExpressionSyntax> Parser::ParseNameExpression()
	{ return make_shared<NameExpressionSyntax>(Match(Token::Identifier)); }

	shared_ptr<LiteralExpressionSyntax> Parser::ParseLiteralExpression()
	{ return make_shared<LiteralExpressionSyntax>(Next()); }

	shared_ptr<ParenthesizedExpressionSyntax> Parser::ParseParenthesizedExpression()
	{
		shared_ptr<Token> open = Match(Token::OpenParenthesis);
		shared_ptr<ExpressionSyntax> expression = ParseExpression();
		shared_ptr<Token> close = Match(Token::CloseParenthesis);

		return make_shared<ParenthesizedExpressionSyntax>(open, expression, close);
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


	bool Parser::IsType()
	{
		switch (Peek()->GetKind())
		{
		case Token::VoidKeyword:
		case Token::ByteKeyword:
		case Token::ShortKeyword:
		case Token::IntKeyword:
		case Token::LongKeyword:

		case Token::BoolKeyword:
		case Token::CharKeyword:
		case Token::WCharKeyword:

		case Token::FloatKeyword:
		case Token::DoubleKeyword:
		case Token::TripleKeyword:

		case Token::Asterisk:
			return true;

		default:
			return false;
		}
	}
}