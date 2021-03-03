#pragma once

#include "Symple/Code/Ast/Ast.h"
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
		GlobalRef<CompilationUnitAst> Parse();
	private:
		GlobalRef<MemberAst> ParseMember();
		GlobalRef<FunctionAst> ParseFunction();

		GlobalRef<StatementAst> ParseStatement();
		GlobalRef<ReturnStatementAst> ParseReturnStatement();
		GlobalRef<ExpressionStatementAst> ParseExpressionStatement();

		GlobalRef<ExpressionAst> ParseExpression();
		GlobalRef<ExpressionAst> ParseBinaryExpression(uint32 parentPrecedence = 0);

		GlobalRef<ExpressionAst> ParsePrimaryExpression();
		GlobalRef<LiteralExpressionAst> ParseLiteralExpression();

		GlobalRef<const Token> Peek();
		GlobalRef<const Token> Peek(uint32 offset);
		GlobalRef<const Token> Next();
		GlobalRef<const Token> Match(TokenKind);

		SY_PROPERTY_GET(Peek) GlobalRef<const Token> Current;
	};
}