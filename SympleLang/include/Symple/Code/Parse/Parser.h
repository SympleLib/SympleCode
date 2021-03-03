#pragma once

#include "Symple/Code/AST/AST.h"
#include "Symple/Code/Parse/Token.h"

namespace Symple::Code
{
	class SYC_API Parser
	{
	private:
		const TokenList m_Tokens;
		uint32 m_Position = 0;
	public:
		Parser(const TokenList &tokens);
		GlobalRef<CompilationUnitAST> Parse();
	private:
		GlobalRef<MemberAST> ParseMember();
		GlobalRef<FunctionAST> ParseFunction();

		GlobalRef<StatementAST> ParseStatement();
		GlobalRef<ExpressionStatementAST> ParseExpressionStatement();

		GlobalRef<ExpressionAST> ParseExpression();
		GlobalRef<LiteralExpressionAST> ParseLiteralExpression();

		GlobalRef<const Token> Peek();
		GlobalRef<const Token> Peek(uint32 offset);
		GlobalRef<const Token> Next();
		GlobalRef<const Token> Match(TokenKind);

		SY_PROPERTY_GET(Peek) GlobalRef<const Token> Current;
	};
}