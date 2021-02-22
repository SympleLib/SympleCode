#pragma once

#include "Symple/Syntax/Syntax.h"
#include "Symple/Syntax/ExpressionSyntax.h"
#include "Symple/Syntax/LiteralExpressionSyntax.h"

namespace Symple
{
	class SY_API Parser
	{
	private:
		uint32 pos;
		std::vector<GlobalRef<Token>> toks;
	public:
		GlobalRef<Syntax> Parse(const std::vector<GlobalRef<Token>> &tokens);
	private:
		GlobalRef<ExpressionSyntax> ParseExpression();
		GlobalRef<LiteralExpressionSyntax> ParseLiteralExpression();

		SY_PROPERTY_GET(Peek) GlobalRef<Token> Current;

		GlobalRef<Token> Peek();
		GlobalRef<Token> Peek(uint32 offset);
		GlobalRef<Token> Next();
		GlobalRef<Token> Match(TokenKind);
	};
}