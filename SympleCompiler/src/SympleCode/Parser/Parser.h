#pragma once

#include <vector>

#include "SympleCode/Parser/Lexer.h"
#include "SympleCode/Common/Node/Node.h"

#include "SympleCode/Common/Node/BlockStatementNode.h"
#include "SympleCode/Common/Node/GlobalStatementNode.h"
#include "SympleCode/Common/Node/ReturnStatementNode.h"
#include "SympleCode/Common/Node/CompilationUnitNode.h"
#include "SympleCode/Common/Node/VariableDeclarationNode.h"

#include "SympleCode/Common/Node/FunctionDeclarationNode.h"
#include "SympleCode/Common/Node/FunctionCallArgumentsNode.h"
#include "SympleCode/Common/Node/FunctionCallExpressionNode.h"

#include "SympleCode/Common/Node/ExpressionNode.h"
#include "SympleCode/Common/Node/LiteralExpressionNode.h"
#include "SympleCode/Common/Node/NumberLiteralExpressionNode.h"
#include "SympleCode/Common/Node/BooleanLiteralExpressionNode.h"

#include "SympleCode/Common/Analysis/Type.h"
#include "SympleCode/Common/Analysis/Diagnostics.h"

namespace Symple
{
	class Parser
	{
	private:
		Lexer mLexer;
		std::vector<const Token*> mTokens;
		std::vector<const Type*> mTypes;
		Diagnostics* mDiagnostics;
		size_t mPosition;
	public:
		Parser(const char* source = "");

		CompilationUnitNode* ParseCompilationUnit();
		const Diagnostics* GetDiagnostics() const;
	private:
		const Token* Peek(size_t offset = 0);
		const Token* Next();
		const Token* Match(Token::Kind kind);

		bool IsType(const Token* token);
		const Type* GetType(const Token* token);

		const std::vector<const MemberNode*> ParseMembers();
		MemberNode* ParseMember();

		FunctionDeclarationNode* ParseFunctionDeclaration();

		StatementNode* ParseStatement();
		BlockStatementNode* ParseBlockStatement();
		ReturnStatementNode* ParseReturnStatement();
		GlobalStatementNode* ParseGlobalStatement();
		VariableDeclarationNode* ParseVariableDeclaration();
	
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