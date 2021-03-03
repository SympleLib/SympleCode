#include "Symple/Code/Parse/Parser.h"

#include <iostream>

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
	{ return ParseFunction(); }

	GlobalRef<FunctionAst> Parser::ParseFunction()
	{
		auto ty = Match(TokenKind::IntKeyword);
		auto name = Match(TokenKind::Identifier);
		auto open = Match(TokenKind::OpenParen);
		auto close = Match(TokenKind::CloseParen);
		Match(TokenKind::EqualArrow);
		auto body = ParseStatement();

		return MakeRef<FunctionAst>(ty, name, open, close, body);
	}


	GlobalRef<StatementAst> Parser::ParseStatement()
	{
		switch (Current->Kind)
		{
		case TokenKind::ReturnKeyword:
			return ParseReturnStatement();
		default:
			return ParseExpressionStatement();
		}
	}

	GlobalRef<ReturnStatementAst> Parser::ParseReturnStatement()
	{
		auto keyword = Match(TokenKind::ReturnKeyword);
		auto expr = ParseExpression();
		Match(TokenKind::Semicolon);
		return MakeRef<ReturnStatementAst>(keyword, expr);
	}

	GlobalRef<ExpressionStatementAst> Parser::ParseExpressionStatement()
	{
		auto expr = ParseExpression();
		Match(TokenKind::Semicolon);
		return MakeRef<ExpressionStatementAst>(expr);
	}


	GlobalRef<ExpressionAst> Parser::ParseExpression()
	{ return ParseLiteralExpression(); }

	GlobalRef<LiteralExpressionAst> Parser::ParseLiteralExpression()
	{
		auto literal = Next();
		return MakeRef<LiteralExpressionAst>(literal);
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