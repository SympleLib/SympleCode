#include "Symple/Common/Common.h"
#include "Symple/Syntax/Parser.h"

namespace Symple
{
	GlobalRef<Syntax> Parser::Parse(const std::vector<GlobalRef<Token>> &toks)
	{
		this->toks = toks;
		return ParseExpression();
	}

	GlobalRef<ExpressionSyntax> Parser::ParseExpression()
	{ return ParseLiteralExpression(); }

	GlobalRef<LiteralExpressionSyntax> Parser::ParseLiteralExpression()
	{ return MakeGlobalRef<LiteralExpressionSyntax>(Next()); }


	GlobalRef<Token> Parser::Peek()
	{ return Peek(0); }

	GlobalRef<Token> Parser::Peek(uint32 off)
	{
		unsigned pos = this->pos + off;
		if (pos >= toks.size())
			return toks.back();
		return toks[pos];
	}

	GlobalRef<Token> Parser::Next()
	{
		auto current = Current;
		pos++;
		return current;
	}

	GlobalRef<Token> Parser::Match(TokenKind kind)
	{
		if (Current->Kind == kind)
			return Next();
		abort();
		return Current;
	}
}