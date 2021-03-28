#include "Symple/Code/Parse/Parser.h"

#include <iostream>

#include "Symple/Code/Util/Conversions.h"
#include "Symple/Code/Parse/Facts.h"

namespace Symple::Code
{
	Parser::Parser(const TokenList &toks)
		: m_Tokens(toks) {}

	GlobalRef<CompilationUnitAst> Parser::Parse()
	{
		MemberList members;
		while (!Current->Is(TokenKind::EndOfFile))
			members.push_back(ParseMember());
		auto eof = Match(TokenKind::EndOfFile);
		return MakeRef<CompilationUnitAst>(members, eof);
	}

	GlobalRef<MemberAst> Parser::ParseMember()
	{
		if (TokenFacts::IsTypeBase(Current->Kind))
			return ParseFunction();
		else
			throw std::exception("Must be function declaration!");
	}

	GlobalRef<FunctionAst> Parser::ParseFunction()
	{
		auto ty = ParseType();
		auto name = Match(TokenKind::Identifier);
		auto open = Match(TokenKind::OpenParen);
		auto params = ParseParameters();
		auto close = Match(TokenKind::CloseParen);
		if (Current->Is(TokenKind::EqualArrow))
			Next();
		auto body = ParseStatement();

		return MakeRef<FunctionAst>(ty, name, open, params, close, body);
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
			if (TokenFacts::IsTypeBase(Current->Kind))
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
			stmts.push_back(ParseStatement());
		auto close = Next();

		return MakeRef<BlockStatementAst>(open, stmts, close);
	}

	GlobalRef<ReturnStatementAst> Parser::ParseReturnStatement()
	{
		auto keyword = Match(TokenKind::ReturnKeyword);
		auto expr = ParseExpression();
		return MakeRef<ReturnStatementAst>(keyword, expr);
	}

	GlobalRef<VariableStatementAst> Parser::ParseVariableStatement()
	{
		auto ty = ParseType();
		auto name = Match(TokenKind::Identifier);
		if (Current->Is(TokenKind::Equal))
		{
			auto equal = Next();
			auto init = ParseExpression();
			return MakeRef<VariableStatementAst>(ty, name, equal, init);
		}
		return MakeRef<VariableStatementAst>(ty, name);
	}

	GlobalRef<ExpressionStatementAst> Parser::ParseExpressionStatement()
	{
		auto expr = ParseExpression();
		Match(TokenKind::Semicolon);
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
			return ParsePrimaryExpression();
	}

	GlobalRef<ExpressionAst> Parser::ParsePostfixExpression(uint32 parentPrecedence)
	{
		auto operand = ParsePrefixExpression(parentPrecedence);
		// Hard coded for now
		if (Current->Is(TokenKind::OpenParen))
			return ParseCallExpression(operand);
		else
			return operand;
	}

	GlobalRef<ExpressionAst> Parser::ParseBinaryExpression(uint32 parentPrecedence)
	{
		auto left = ParsePostfixExpression();
		while (true)
		{
			uint32 precedence = Precedence::Binary(Current->Kind);
			if (!precedence || precedence <= parentPrecedence)
				return left;
			auto op = Next();
			auto right = ParsePostfixExpression(precedence);
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
			Match(TokenKind::Comma);
			args.push_back(ParseExpression());
		}
		auto close = Next();

		return MakeRef<CallExpressionAst>(func, open, args, close);
	}

	GlobalRef<ExpressionAst> Parser::ParsePrimaryExpression()
	{
		switch (Current->Kind)
		{
		case TokenKind::Identifier:
			return ParseNameExpression();
		case TokenKind::Number:
			return ParseLiteralExpression();
		case TokenKind::OpenParen:
		{
			auto open = Next();
			if (TokenFacts::IsTypeBase(Current->Kind))
				return ParseCastExpression(open);
			else
				return ParseParenthasizedExpression(open);
		}
		}
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
		assert(TokenFacts::IsTypeBase(base->Kind));
		ConstWeakTokenList addons;
		while (TokenFacts::IsTypePointer(Current->Kind))
			addons.push_back(Next());
		bool isRef = false;
		if (TokenFacts::IsTypeRef(Current->Kind))
		{
			addons.push_back(Next());
			isRef = true;
		}

		auto tyKind = TokenFacts::ToType(base->Kind);
		GlobalRef<Type> ty = MakeRef<Type>(tyKind, addons.size() - isRef, isRef);

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
			Match(TokenKind::Comma);
			auto param = ParseParameter(ty);
			ty = param->m_Type;
			params.push_back(param);
		}

		return params;
	}

	GlobalRef<ParameterAst> Parser::ParseParameter(GlobalRef<TypeAst> ty)
	{
		if (!ty || TokenFacts::IsTypeBase(Current->Kind))
			ty = ParseType();
		auto name = Match(TokenKind::Identifier);

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
		std::cerr << '[' << Current->DisplayLine << ':' << Current->Column << "]: " << "Unexpected " << Current->Kind << " '', Expected " << kind << '\n';
		return Current;
	}
}