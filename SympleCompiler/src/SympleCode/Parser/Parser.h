#pragma once

#include <vector>

#include "SympleCode/Parser/Lexer.h"
#include "SympleCode/Common/Node/Node.h"

#include "SympleCode/Common/Node/BlockStatementNode.h"
#include "SympleCode/Common/Node/GlobalStatementNode.h"
#include "SympleCode/Common/Node/CompilationUnitNode.h"

#include "SympleCode/Common/Node/FunctionDeclarationNode.h"
#include "SympleCode/Common/Node/FunctionCallArgumentsNode.h"
#include "SympleCode/Common/Node/FunctionCallExpressionNode.h"

#include "SympleCode/Common/Node/ExpressionNode.h"
#include "SympleCode/Common/Node/LiteralExpressionNode.h"
#include "SympleCode/Common/Node/NumberLiteralExpressionNode.h"
#include "SympleCode/Common/Node/BooleanLiteralExpressionNode.h"

namespace Symple
{
	class Parser
	{
	private:
		Lexer mLexer;
		std::vector<const Token*> mTokens;
		size_t mPosition;
	public:
		Parser(const char* source = "");

		CompilationUnitNode* ParseCompilationUnit();
	private:
		const Token* Peek(size_t offset = 0);
		const Token* Next();
		const Token* Match(Token::Kind kind);

		const std::vector<const MemberNode*> ParseMembers();
		MemberNode* ParseMember();

		FunctionDeclarationNode* ParseFunctionDeclaration();

		GlobalStatementNode* ParseGlobalStatement();
		BlockStatementNode* ParseBlockStatement();
		StatementNode* ParseStatement();
	
		ExpressionNode* ParseExpression();

		ExpressionNode* ParseBinaryExpression(int parentPriority = -1);
		ExpressionNode* ParsePrimaryExpression();

		ExpressionNode* ParseNameOrCallExpression();
		FunctionCallExpressionNode* ParseFunctionCallExpression();
		FunctionCallArgumentsNode* ParseFunctionCallArguments();

		NumberLiteralExpressionNode* ParseNumberLiteral();
		BooleanLiteralExpressionNode* ParseBooleanLiteral();
	};
}