#pragma once

#include <string>
#include <vector>

#include "SympleCode/Parser/Preprocessor.h"

#include "SympleCode/Common/Node/Node.h"
#include "SympleCode/Common/Node/CompilationUnitNode.h"

#include "SympleCode/Common/Node/Statement/IfStatementNode.h"
#include "SympleCode/Common/Node/Statement/BlockStatementNode.h"
#include "SympleCode/Common/Node/Statement/WhileStatementNode.h"
#include "SympleCode/Common/Node/Statement/ReturnStatementNode.h"
#include "SympleCode/Common/Node/Statement/VariableDeclarationNode.h"

#include "SympleCode/Common/Node/Member/FunctionHintNode.h"
#include "SympleCode/Common/Node/Member/ExternFunctionNode.h"
#include "SympleCode/Common/Node/Member/GlobalStatementNode.h"
#include "SympleCode/Common/Node/Member/FunctionDeclarationNode.h"

#include "SympleCode/Common/Node/Expression/ExpressionNode.h"
#include "SympleCode/Common/Node/Expression/ModifiableExpressionNode.h"
#include "SympleCode/Common/Node/Expression/FunctionCallExpressionNode.h"
#include "SympleCode/Common/Node/Expression/ParenthesizedExpressionNode.h"

#include "SympleCode/Common/Node/Expression/Literal/LiteralExpressionNode.h"

#include "SympleCode/Common/Type.h"
#include "SympleCode/Common/Analysis/Diagnostics.h"

namespace Symple
{
	class Parser
	{
	private:
		Preprocessor mPreprocessor;

		std::vector<const Token*> mTokens;
		std::vector<const Type*> mTypes;
		size_t mPosition;

		std::map<std::string, const VariableDeclarationNode*> mDeclaredVariables;
	public:
		Parser(const char* source = "");

		CompilationUnitNode* ParseCompilationUnit();
	private:
		const Token* Peek(unsigned int offset = 0) const;
		const Token* Next();
		const Token* Match(Token::Kind kind);

		bool IsType(const Token* token) const;
		const Type* GetType(const Token* token) const;

		bool IsTypeNodeable(const Token* token) const;
		TypeNode* ParseType();

		const std::vector<const MemberNode*> ParseMembers();
		MemberNode* ParseMember();

		FunctionDeclarationNode* ParseFunctionDeclaration();
		FunctionArgumentsNode* ParseFunctionArguments();
		FunctionModifiersNode* ParseFunctionModifiers();
		FunctionModifierNode* ParseFunctionModifier();
		FunctionArgumentNode* ParseFunctionArgument();
		ExternFunctionNode* ParseExternFunction();
		FunctionHintNode* ParseFunctionHint();

		StatementNode* ParseStatement();
		IfStatementNode* ParseIfStatement();
		WhileStatementNode* ParseWhileStatement();
		BlockStatementNode* ParseBlockStatement();
		ReturnStatementNode* ParseReturnStatement();
		GlobalStatementNode* ParseGlobalStatement();
		VariableDeclarationNode* ParseVariableDeclaration(const TypeNode* type = nullptr);
		VariableModifiersNode* ParseVariableModifiers();
		VariableModifierNode* ParseVariableModifier();
	
		ExpressionNode* ParseExpression();

		ExpressionNode* ParseAssignmentExpression();
		ExpressionNode* ParseUnaryExpression(int parentPriority = -1);
		ExpressionNode* ParseBinaryExpression(int parentPriority = -1);
		ExpressionNode* ParsePrimaryExpression();

		ExpressionNode* ParseNameOrCallExpression();
		ModifiableExpressionNode* ParseModifiableExpression();
		ModifiableExpressionNode* ParsePointerIndexExpression();
		FunctionCallArgumentsNode* ParseFunctionCallArguments();

		FunctionCallExpressionNode* ParseFunctionCallExpression();
		ParenthesizedExpressionNode* ParseParenthesizedExpression();
	};
}