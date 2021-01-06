#pragma once

#include <string>
#include <vector>

#include "SympleCode/Parser/Preprocessor.h"

#include "SympleCode/Node/Node.h"
#include "SympleCode/Node/CompilationUnitNode.h"

#include "SympleCode/Node/Statement/IfStatementNode.h"
#include "SympleCode/Node/Statement/BlockStatementNode.h"
#include "SympleCode/Node/Statement/WhileStatementNode.h"
#include "SympleCode/Node/Statement/ReturnStatementNode.h"
#include "SympleCode/Node/Statement/VariableDeclarationNode.h"

#include "SympleCode/Node/Member/FunctionHintNode.h"
#include "SympleCode/Node/Member/ExternFunctionNode.h"
#include "SympleCode/Node/Member/GlobalStatementNode.h"
#include "SympleCode/Node/Member/StructDeclarationNode.h"
#include "SympleCode/Node/Member/FunctionDeclarationNode.h"
#include "SympleCode/Node/Member/GlobalVariableDeclarationNode.h"

#include "SympleCode/Node/Expression/ExpressionNode.h"
#include "SympleCode/Node/Expression/CastExpressionNode.h"
#include "SympleCode/Node/Expression/FunctionCallExpressionNode.h"
#include "SympleCode/Node/Expression/ParenthesizedExpressionNode.h"

#include "SympleCode/Node/Expression/Literal/LiteralExpressionNode.h"

#include "SympleCode/Node/Expression/Modifiable/DereferencePointerExpressionNode.h"

namespace Symple
{
	class Parser
	{
	private:
		Preprocessor mPreprocessor;

		std::vector<const Token*> mTokens;
		std::vector<const Type*> mTypes;
		size_t mPosition;
	public:
		Parser(const char* source = "", const char* file = "");

		CompilationUnitNode* ParseCompilationUnit();
	private:
		const Token* Peek(unsigned int offset = 0) const;
		const Token* Next();
		const Token* Match(Token::Kind kind);

		static bool IsType(const Token* token);
		static const Type* GetType(const Token* token);

		static bool IsTypeNodeable(const Token* token);
		static bool IsTypeContinue(const Token* token);
		TypeNode* ParseType();
		TypeContinueNode* ParseTypeContinue();

		const std::vector<const MemberNode*> ParseMembers();
		MemberNode* ParseMember();

		FunctionDeclarationNode* ParseFunctionDeclaration();
		FunctionArgumentsNode* ParseFunctionArguments();
		FunctionModifiersNode* ParseFunctionModifiers();
		FunctionModifierNode* ParseFunctionModifier();
		FunctionArgumentNode* ParseFunctionArgument();
		ExternFunctionNode* ParseExternFunction();
		FunctionHintNode* ParseFunctionHint();
		GlobalVariableDeclarationNode* ParseGlobalVariableDeclaration(const TypeNode* type = nullptr);
		StructDeclarationNode* ParseStructDeclaration();
		FieldListNode* ParseFieldList();
		VariableDeclarationNode* ParseField(const TypeNode* type = nullptr);

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

		CastExpressionNode* ParseCastExpression();
		ParenthesizedExpressionNode* ParseParenthesizedExpression();

		ModifiableExpressionNode* ParseModifiableExpression();
		VariableExpressionNode* ParseVariableExpression();
		VariableAddressExpressionNode* ParseVariableAddressExpression();
		DereferencePointerExpressionNode* ParseDereferencePointerExpression();

		FunctionCallArgumentsNode* ParseFunctionCallArguments();
		FunctionCallExpressionNode* ParseFunctionCallExpression();
	};
}