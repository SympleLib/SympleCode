#include "SympleCode/Syntax/Parser.h"

#include <sstream>

#include <spdlog/spdlog.h>

#include "SympleCode/Syntax/Facts.h"
#include "SympleCode/Syntax/GlobalStatementSyntax.h"

namespace Symple::Syntax
{
	Parser::Parser(shared_ptr<Lexer> lexer)
	{
		do
			mTokens.push_back(lexer->Lex());
		while (!mTokens.back()->Is(Token::EndOfFile));
	}

	Parser::Parser(shared_ptr<Lexer> lexer, TokenList tokens)
		: mTokens(tokens)
	{
		if (!tokens.empty() && tokens.back()->Is(Token::EndOfFile))
			return;

		do
			mTokens.push_back(lexer->Lex());
		while (!mTokens.back()->Is(Token::EndOfFile));
	}

	Parser::Parser(TokenList tokens)
		: mTokens(tokens)
	{}


	shared_ptr<TranslationUnitSyntax> Parser::Parse()
	{
		std::vector<shared_ptr<MemberSyntax>> members;
		while (!Peek()->Is(Token::EndOfFile))
		{
			unsigned start = mPosition;
			members.push_back(ParseMember());
			if (start == mPosition)
				Next();
		}
		shared_ptr<Token> eof = Match(Token::EndOfFile);

		return make_shared<TranslationUnitSyntax>(members, eof);
	}


	shared_ptr<MemberSyntax> Parser::ParseMember()
	{
		if (IsType())
			return ParseFunctionDeclaration();
		else
			switch (Peek()->GetKind())
			{
			case Token::ExternKeyword:
				return ParseExternFunction();
			case Token::StructKeyword:
				return ParseStructDeclaration();
			case Token::ImportKeyword:
				return ParseImportStatement();
			default:
				return make_shared<GlobalStatementSyntax>(ParseStatement());
			}
	}

	shared_ptr<ExternFunctionSyntax> Parser::ParseExternFunction()
	{
		shared_ptr<Token> keyword = Match(Token::ExternKeyword);
		auto type = ParseType();
		shared_ptr<Token> name = Match(Token::Identifier);
		shared_ptr<Token> openParen = Match(Token::OpenParenthesis);
		auto params = ParseFunctionParameters();
		shared_ptr<Token> closeParen = Match(Token::CloseParenthesis);
		TokenList modifiers = ParseFunctionModifiers();
		Match(Token::Semicolon);

		return make_shared<ExternFunctionSyntax>(keyword, type, name, openParen, params, closeParen, modifiers);
	}

	shared_ptr<FunctionDeclarationSyntax> Parser::ParseFunctionDeclaration()
	{
		auto type = ParseType();
		shared_ptr<Token> name = Match(Token::Identifier);
		shared_ptr<Token> openParen = Match(Token::OpenParenthesis);
		auto params = ParseFunctionParameters();
		shared_ptr<Token> closeParen = Match(Token::CloseParenthesis);
		TokenList modifiers = ParseFunctionModifiers();
		shared_ptr<StatementSyntax> statement = ParseStatement();

		return make_shared<FunctionDeclarationSyntax>(type, name, openParen, params, closeParen, modifiers, statement);
	}

	VariableDeclarationList Parser::ParseFunctionParameters()
	{
		VariableDeclarationList list;

		bool first = true;
		shared_ptr<TypeSyntax> pty;
		while (!Peek()->Is(Token::CloseParenthesis))
		{
			if (Peek()->Is(Token::EndOfFile))
			{
				mDiagnosticBag->ReportUnexpectedEndOfFile(Peek());
				break;
			}

			if (first)
				first = false;
			else
				Match(Token::Comma);

			if (!Peek()->Is(Token::CloseParenthesis))
			{
				list.push_back(ParseVariableDeclaration(pty));
				pty = list.back()->GetType();
			}
		}

		return list;
	}

	TokenList Parser::ParseFunctionModifiers()
	{
		TokenList list;

		while (Peek()->Is(Token::CDeclKeyword, Token::StdCallKeyword,           // Naming Conventions
			Token::DllExportKeyword, Token::DllImportKeyword,                   // Dll Stuff
			Token::StaticKeyword, Token::LocalKeyword, Token::GlobalKeyword,    // Visibility Modifiers
			Token::Comma))                                                      // Commas
		{
			if (Peek()->Is(Token::Comma))
				Next();
			else
				list.push_back(Next());
		}

		return list;
	}

	shared_ptr<StructDeclarationSyntax> Parser::ParseStructDeclaration()
	{
		shared_ptr<Token> keyword = Match(Token::StructKeyword);
		shared_ptr<Token> name = Match(Token::Identifier);
		shared_ptr<Token> open = Match(Token::OpenBrace);
		auto members = ParseStructMembers();
		shared_ptr<Token> close = Match(Token::CloseBrace);

		return make_shared<StructDeclarationSyntax>(keyword, name, open, members, close);
	}

	VariableDeclarationList Parser::ParseStructMembers()
	{
		VariableDeclarationList list;

		bool first = true;
		shared_ptr<TypeSyntax> pty;
		while (!Peek()->Is(Token::CloseBrace))
		{
			if (Peek()->Is(Token::EndOfFile))
			{
				mDiagnosticBag->ReportUnexpectedEndOfFile(Peek());
				break;
			}

			if (first)
				first = false;
			else
				Match(Token::Comma);

			if (!Peek()->Is(Token::CloseBrace))
			{
				list.push_back(ParseVariableDeclaration(pty));
				pty = list.back()->GetType();
			}
		}

		return list;
	}

	shared_ptr<ImportStatementSyntax> Parser::ParseImportStatement()
	{
		shared_ptr<Token> tok = Match(Token::ImportKeyword);
		shared_ptr<Token> import = Match(Token::String);
		Match(Token::Semicolon);

		return make_shared<ImportStatementSyntax>(tok, import);
	}


	shared_ptr<StatementSyntax> Parser::ParseStatement(bool matchSemi)
	{
		shared_ptr<StatementSyntax> statement;

		if (IsType())
			statement = ParseVariableDeclaration();
		else
			switch (Peek()->GetKind())
			{
			case Token::IfKeyword:
				statement = ParseIfStatement();
				matchSemi = false;
				break;
			case Token::GotoKeyword:
				statement = ParseGotoStatement();
				break;
			case Token::NativeKeyword:
				statement = ParseNativeStatement();
				break;
			case Token::OpenBrace:
				statement = ParseBlockStatement();
				matchSemi = false;
				break;
			case Token::ReturnKeyword:
				statement = ParseReturnStatement();
				break;
			case Token::Identifier:
				if (Peek(1)->Is(Token::Colon))
				{
					statement = ParseLabel();
					matchSemi = false;
					break;
				}

			default:
				statement = ParseExpressionStatement();
				break;
			}

		if (matchSemi)
			Match(Token::Semicolon);
		return statement;
	}

	shared_ptr<LabelSyntax> Parser::ParseLabel()
	{
		shared_ptr<Token> label = Match(Token::Identifier);
		shared_ptr<Token> colon = Match(Token::Colon);

		return make_shared<LabelSyntax>(label, colon);
	}

	shared_ptr<IfStatementSyntax> Parser::ParseIfStatement()
	{
		shared_ptr<Token> ifKey = Match(Token::IfKeyword);
		shared_ptr<ParenthesizedExpressionSyntax> cond = ParseParenthesizedExpression();
		shared_ptr<StatementSyntax> then = ParseStatement();

		shared_ptr<Token> elseKey;
		shared_ptr<StatementSyntax> elze;
		if (Peek()->Is(Token::ElseKeyword))
		{
			elseKey = Next();
			elze = ParseStatement();
		}

		return make_shared<IfStatementSyntax>(ifKey, cond, then, elseKey, elze);
	}

	shared_ptr<GotoStatementSyntax> Parser::ParseGotoStatement()
	{
		shared_ptr<Token> keyword = Match(Token::GotoKeyword);
		shared_ptr<Token> label = Match(Token::Identifier);

		return make_shared<GotoStatementSyntax>(keyword, label);
	}

	shared_ptr<NativeStatementSyntax> Parser::ParseNativeStatement()
	{
		shared_ptr<Token> tok = Match(Token::NativeKeyword);
		shared_ptr<Token> code = Match(Token::String);

		return make_shared<NativeStatementSyntax>(tok, code);
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

	shared_ptr<ExpressionStatementSyntax> Parser::ParseExpressionStatement()
	{
		shared_ptr<ExpressionSyntax> expr = ParseExpression();

		return make_shared<ExpressionStatementSyntax>(expr);
	}

	shared_ptr<VariableDeclarationSyntax> Parser::ParseVariableDeclaration(shared_ptr<TypeSyntax> ty)
	{
		if (!ty || IsType())
			ty = ParseType();
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

		return make_shared<VariableDeclarationSyntax>(ty, name, equals, initializer);
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

	ExpressionList Parser::ParseExpressionList()
	{
		ExpressionList list;

		bool first = true;
		while (!Peek()->Is(Token::CloseParenthesis, Token::CloseBrace))
		{
			if (Peek()->Is(Token::EndOfFile))
			{
				mDiagnosticBag->ReportUnexpectedEndOfFile(Peek());
				break;
			}

			if (first)
				first = false;
			else
				Match(Token::Comma);
			list.push_back(ParseExpression());
		}

		return list;
	}

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
			if (Peek(1)->Is(Token::OpenParenthesis))
				return ParseCallExpression();
			else
				return ParseNameExpression();
		case Token::Number:
		case Token::Float:
		case Token::Integer:
		case Token::String:
		case Token::DefaultKeyword:
			return ParseLiteralExpression();
		case Token::OpenParenthesis:
			return ParseParenthesizedExpression();

		default:
			return make_shared<ExpressionSyntax>(Next());
		}
	}

	shared_ptr<NameExpressionSyntax> Parser::ParseNameExpression()
	{ return make_shared<NameExpressionSyntax>(Match(Token::Identifier)); }

	shared_ptr<CallExpressionSyntax> Parser::ParseCallExpression()
	{
		shared_ptr<Token> name = Match(Token::Identifier);
		shared_ptr<Token> open = Match(Token::OpenParenthesis);
		ExpressionList args = ParseExpressionList();
		shared_ptr<Token> close = Match(Token::CloseParenthesis);

		return make_shared<CallExpressionSyntax>(name, open, args, close);
	}

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