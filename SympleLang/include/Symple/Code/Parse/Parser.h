#pragma once

#include "Symple/Code/Ast/Ast.h"
#include "Symple/Code/Parse/Token.h"
#include "Symple/Code/BugCheck/ErrorList.h"

namespace Symple::Code
{
	class SYC_API Parser
	{
	private:
		const TokenList m_Tokens;
		uint32 m_Position = 0;

		ErrorList m_ErrorList;
	public:
		Parser(const TokenList &tokens);
		GlobalRef<CompilationUnitAst> Parse(Scope<ErrorList> *);
	private:
		GlobalRef<MemberAst> ParseMember();
		GlobalRef<ProtoAst> ParseProto();
		GlobalRef<FunctionAst> ParseFunction();
		GlobalRef<ExternFunctionAst> ParseExternFunction();
		ConstTokenList ParseFunctionModifiers();

		GlobalRef<StatementAst> ParseStatement(bool matchSemicolon = true);
		GlobalRef<BlockStatementAst> ParseBlockStatement();
		GlobalRef<ReturnStatementAst> ParseReturnStatement();
		GlobalRef<VariableStatementAst> ParseVariableStatement();
		GlobalRef<ExpressionStatementAst> ParseExpressionStatement();

		GlobalRef<ExpressionAst> ParseExpression();
		GlobalRef<ExpressionAst> ParsePrefixExpression(uint32 parentPrecedence = 0);
		GlobalRef<ExpressionAst> ParsePostfixExpression(uint32 parentPrecedence = 0);
		GlobalRef<ExpressionAst> ParseBinaryExpression(uint32 parentPrecedence = 0);
		GlobalRef<CallExpressionAst> ParseCallExpression(GlobalRef<ExpressionAst> function = nullptr);

		GlobalRef<ExpressionAst> ParsePrimaryExpression();
		GlobalRef<PunExpressionAst> ParsePunExpression();
		GlobalRef<CastExpressionAst> ParseCastExpression(GlobalRef<const Token> open = nullptr);
		GlobalRef<NameExpressionAst> ParseNameExpression();
		GlobalRef<LiteralExpressionAst> ParseLiteralExpression();
		GlobalRef<ParenthasizedExpressionAst> ParseParenthasizedExpression(GlobalRef<const Token> open = nullptr);

		GlobalRef<TypeAst> ParseType();
		ParameterList ParseParameters();
		GlobalRef<ParameterAst> ParseParameter(GlobalRef<TypeAst> type = nullptr);

		GlobalRef<const Token> Peek();
		GlobalRef<const Token> Peek(uint32 offset);
		GlobalRef<const Token> Next();
		GlobalRef<const Token> Match(TokenKind);

		SY_PROPERTY_GET(Peek) GlobalRef<const Token> Current;
	};
}