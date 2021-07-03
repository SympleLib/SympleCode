#include "Symple/Code/Parse/Parser.h"

#include <iostream>

#include "Symple/Code/Util/Conversions.h"
#include "Symple/Code/Parse/Facts.h"

namespace Symple::Code
{
	Parser::Parser(const TokenList &toks)
		: m_Tokens(toks), m_Buddy(MakeRef<SymbolBuddy>()) {}

	GlobalRef<CompilationUnitAst> Parser::Parse(ErrorList *errorList)
	{
		m_ErrorList = errorList;

		MemberList members;
		while (!Current->Is(TokenKind::EndOfFile))
			members.push_back(ParseMember());

		auto eof = Match(TokenKind::EndOfFile);
		return MakeRef<CompilationUnitAst>(members, eof);
	}

	GlobalRef<MemberAst> Parser::ParseMember()
	{
		if (m_Buddy->IsTypeBase(Current))
			return ParseFunction();
		else if (Current->Is(TokenKind::ExternKeyword))
			return ParseExternFunction();
		else if (Current->Is(TokenKind::ProtoKeyword))
			return ParseProto();
		else
			throw "Must be function declaration!";
	}

	GlobalRef<ProtoAst> Parser::ParseProto()
	{
		Match(TokenKind::ProtoKeyword);
		auto name = Match(TokenKind::Identifier);
		// TODO: something
		ConstTokenList mods;
		if (Current->Is(TokenKind::EqualArrow))
			Next();
		auto body = ParseStatement();

		return MakeRef<ProtoAst>(name, mods, body);
	}

	GlobalRef<FunctionAst> Parser::ParseFunction()
	{
		auto ty = ParseType();
		auto name = Match(TokenKind::Identifier);
		auto open = Match(TokenKind::OpenParen);
		auto params = ParseParameters();
		auto close = Match(TokenKind::CloseParen);
		auto mods = ParseFunctionModifiers();
		if (Current->Is(TokenKind::EqualArrow))
			Next();
		auto body = ParseStatement();

		return MakeRef<FunctionAst>(ty, name, open, params, close, mods, body);
	}

	GlobalRef<ExternFunctionAst> Parser::ParseExternFunction()
	{
		auto key = Match(TokenKind::ExternKeyword);
		auto ty = ParseType();
		auto name = Match(TokenKind::Identifier);
		auto open = Match(TokenKind::OpenParen);
		auto params = ParseParameters();
		auto close = Match(TokenKind::CloseParen);
		auto mods = ParseFunctionModifiers();
		Match(TokenKind::Semicolon);

		return MakeRef<ExternFunctionAst>(key, ty, name, open, params, close, mods);
	}

	ConstTokenList Parser::ParseFunctionModifiers()
	{
		ConstTokenList mods;
		while (TokenFacts::IsFuncMod(Current->Kind))
		{
			mods.push_back(Next());
			if (Current->Is(TokenKind::Comma))
				Next();
		}

		return mods;
	}


	GlobalRef<StatementAst> Parser::ParseStatement(bool matchSemi)
	{
		GlobalRef<StatementAst> stmt;
		switch (Current->Kind)
		{
		case TokenKind::Semicolon:
			return MakeRef<EmptyStatementAst>(Next());
		case TokenKind::OpenBrace:
			return ParseBlockStatement();
		case TokenKind::ReturnKeyword:
			stmt = ParseReturnStatement();
			break;
		default:
			if (m_Buddy->IsTypeBase(Current))
				stmt = ParseVariableStatement();
			else
				stmt = ParseExpressionStatement();
			break;
		}

		if (matchSemi)
			Match(TokenKind::Semicolon);
		return stmt;
	}

	GlobalRef<BlockStatementAst> Parser::ParseBlockStatement()
	{
		auto open = Match(TokenKind::OpenBrace);
		StatementList stmts;
		while (!Current->Is(TokenKind::CloseBrace))
		{
			if (Current->Is(TokenKind::EndOfFile))
				throw m_ErrorList->ReportEndOfFile(Current);

			stmts.push_back(ParseStatement());
		}
		auto close = Next();

		return MakeRef<BlockStatementAst>(open, stmts, close);
	}

	GlobalRef<ReturnStatementAst> Parser::ParseReturnStatement()
	{
		auto keyword = Match(TokenKind::ReturnKeyword);
		auto expr = ParseExpression();
		return MakeRef<ReturnStatementAst>(keyword, expr);
	}

	GlobalRef<VariableStatementAst> Parser::ParseVariableStatement(GlobalRef<TypeAst> ty)
	{
		if (!ty || m_Buddy->IsTypeBase(Current))
			ty = ParseType();
		auto name = Match(TokenKind::Identifier);

		GlobalRef<VariableStatementAst> stmt;
		if (Current->Is(TokenKind::Equal))
		{
			auto equal = Next();
			auto init = ParseExpression();
			stmt = MakeRef<VariableStatementAst>(ty, name, equal, init);

			// I'm gonna do whats called a pro-gamer move
			goto TryNext;
		}

		stmt = MakeRef<VariableStatementAst>(ty, name);

	TryNext:
		if (Current->Is(TokenKind::Comma))
		{
			Next();
			auto next = ParseVariableStatement(ty);
			stmt->Next = next;
		}

		return stmt;
	}

	GlobalRef<ExpressionStatementAst> Parser::ParseExpressionStatement()
	{
		auto expr = ParseExpression();
		return MakeRef<ExpressionStatementAst>(expr);
	}


	GlobalRef<ExpressionAst> Parser::ParseExpression()
	{ return ParseBinaryExpression(); }

	GlobalRef<ExpressionAst> Parser::ParsePrefixExpression(uint32 parentPrecedence)
	{
		uint32 precedence = Precedence::Unary(Current->Kind);
		if (precedence && precedence >= parentPrecedence)
		{
			auto op = Next();
			auto operand = ParseBinaryExpression(precedence);

			return MakeRef<UnaryExpressionAst>(op, operand);
		}
		else
			return ParsePostfixExpression(parentPrecedence);
	}

	GlobalRef<ExpressionAst> Parser::ParsePostfixExpression(uint32 parentPrecedence)
	{
		auto operand = ParsePrimaryExpression();
		// Hard coded for now
		while (true)
			switch (Current->Kind)
			{
			case TokenKind::OpenParen:
				operand = ParseCallExpression(operand);
				break;
			default:
				return operand;
			}
	}

	GlobalRef<ExpressionAst> Parser::ParseBinaryExpression(uint32 parentPrecedence)
	{
		auto left = ParsePrefixExpression();
		while (true)
		{
			uint32 precedence = Precedence::Binary(Current->Kind);
			if (!precedence || precedence <= parentPrecedence)
				return left;
			auto op = Next();
			auto right = ParsePostfixExpression(precedence); // TODO: change
			left = MakeRef<BinaryExpressionAst>(op, left, right);
		}
	}

	GlobalRef<CallExpressionAst> Parser::ParseCallExpression(GlobalRef<ExpressionAst> func)
	{
		auto open = Match(TokenKind::OpenParen);
		ExpressionList args;
		if (!Current->Is(TokenKind::CloseParen))
			args.push_back(ParseExpression());
		while (!Current->Is(TokenKind::CloseParen))
		{
			if (Current->Is(TokenKind::EndOfFile))
			{
				m_ErrorList->ReportEndOfFile(Current);
				break;
			}

			Match(TokenKind::Comma);
			args.push_back(ParseExpression());
		}
		auto close = Next();

		return MakeRef<CallExpressionAst>(func, open, args, close);
	}

	GlobalRef<BuiltinExpressionAst> Parser::ParseBuiltinExpression()
	{
		auto name = Next();
		auto open = Match(TokenKind::OpenParen);
		ExpressionList args;
		if (!Current->Is(TokenKind::CloseParen))
			args.push_back(ParseExpression());
		while (!Current->Is(TokenKind::CloseParen))
		{
			if (Current->Is(TokenKind::EndOfFile))
			{
				m_ErrorList->ReportEndOfFile(Current);
				break;
			}

			Match(TokenKind::Comma);
			args.push_back(ParseExpression());
		}
		auto close = Next();

		return MakeRef<BuiltinExpressionAst>(name, name->Kind, open, args, close);
	}

	GlobalRef<ExpressionAst> Parser::ParsePrimaryExpression()
	{
		switch (Current->Kind)
		{
		case TokenKind::Identifier:
			return ParseNameExpression();
		case TokenKind::Char:
		case TokenKind::Number:
		case TokenKind::String:
			return ParseLiteralExpression();
		case TokenKind::OpenBrace:
			return ParsePunExpression();
		case TokenKind::OpenParen:
		{
			auto open = Next();
			if (m_Buddy->IsTypeBase(Current))
				return ParseCastExpression(open);
			else
				return ParseParenthasizedExpression(open);
		}

		default:
			if (TokenFacts::IsBuiltinFunction(Current->Kind))
				return ParseBuiltinExpression();
			else if (Current->Is(TokenKind::Identifier)) // Identifier Keywords
				return ParseNameExpression();
			else
			{
				m_ErrorList->ReportExpectedExpression(Current);
				return MakeRef<ExpressionAst>();
			}
		}
	}

	GlobalRef<PunExpressionAst> Parser::ParsePunExpression()
	{
		auto open = Match(TokenKind::OpenBrace);
		auto ty = ParseType();
		auto close = Match(TokenKind::CloseBrace);
		auto val = ParsePostfixExpression();

		return MakeRef<PunExpressionAst>(open, ty, close, val);
	}

	GlobalRef<CastExpressionAst> Parser::ParseCastExpression(GlobalRef<const Token> open)
	{
		if (!open)
			open = Match(TokenKind::OpenParen);
		auto ty = ParseType();
		auto close = Match(TokenKind::CloseParen);
		auto val = ParsePostfixExpression();

		return MakeRef<CastExpressionAst>(open, ty, close, val);
	}

	GlobalRef<NameExpressionAst> Parser::ParseNameExpression()
	{
		auto name = Match(TokenKind::Identifier);
		return MakeRef<NameExpressionAst>(name);
	}

	GlobalRef<LiteralExpressionAst> Parser::ParseLiteralExpression()
	{
		auto literal = Next();
		return MakeRef<LiteralExpressionAst>(literal);
	}

	GlobalRef<ParenthasizedExpressionAst> Parser::ParseParenthasizedExpression(GlobalRef<const Token> open)
	{
		if (!open)
			open = Match(TokenKind::OpenParen);
		auto expr = ParseExpression();
		auto close = Match(TokenKind::CloseParen);
		return MakeRef<ParenthasizedExpressionAst>(open, expr, close);
	}


	GlobalRef<TypeAst> Parser::ParseType()
	{
		auto base = Next();
		ConstWeakTokenList addons;
		while (TokenFacts::IsTypePointer(Current->Kind))
			addons.push_back(Next());

		auto tyBase = m_Buddy->GetTypeBase(base);
		GlobalRef<Type> ty = MakeRef<Type>(base, addons.size());
		return MakeRef<TypeAst>(base, addons, ty);
	}

	ParameterList Parser::ParseParameters()
	{
		GlobalRef<TypeAst> ty;
		ParameterList params;
		if (!Current->Is(TokenKind::CloseParen))
		{
			auto param = ParseParameter(ty);
			ty = param->m_Type;
			params.push_back(param);
		}
		while (!Current->Is(TokenKind::CloseParen))
		{
			if (Current->Is(TokenKind::EndOfFile))
			{
				m_ErrorList->ReportEndOfFile(Current);
				break;
			}

			Match(TokenKind::Comma);
			auto param = ParseParameter(ty);
			ty = param->m_Type;
			params.push_back(param);
		}

		return params;
	}

	GlobalRef<ParameterAst> Parser::ParseParameter(GlobalRef<TypeAst> ty)
	{
		if (!ty || m_Buddy->IsTypeBase(Current))
			ty = ParseType();
		GlobalRef<const Token> name = nullptr;
		if (Current->Is(TokenKind::Identifier))
			name = Next();

		return MakeRef<ParameterAst>(ty, name);
	}


	GlobalRef<const Token> Parser::Peek()
	{ return std::move(Peek(0)); }

	GlobalRef<const Token> Parser::Peek(uint32 off)
	{
		uint32 pos = m_Position + off;
		if (pos >= m_Tokens.size())
			return m_Tokens.back();
		return m_Tokens[pos];
	}

	GlobalRef<const Token> Parser::Next()
	{
		auto current = Current;
		m_Position++;
		return current;
	}

	GlobalRef<const Token> Parser::Match(TokenKind kind)
	{
		if (Current->Is(kind))
			return Next();

		m_ErrorList->ReportWrongToken(Current, kind);
		return Current;
	}
}