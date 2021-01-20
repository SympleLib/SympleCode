#pragma once

#include <string>
#include <vector>

#include "SympleCode/Parser/Preprocessor.h"

#include "SympleCode/Node/Node.h"
#include "SympleCode/Node/CompilationUnitNode.h"

#include "SympleCode/Node/Statement/IfStatementNode.h"
#include "SympleCode/Node/Statement/AsmStatementNode.h"
#include "SympleCode/Node/Statement/BlockStatementNode.h"
#include "SympleCode/Node/Statement/WhileStatementNode.h"
#include "SympleCode/Node/Statement/ReturnStatementNode.h"
#include "SympleCode/Node/Statement/BreakStatementNode.h"
#include "SympleCode/Node/Statement/ForLoopStatementNode.h"
#include "SympleCode/Node/Statement/VariableDeclarationNode.h"

#include "SympleCode/Node/Member/FunctionHintNode.h"
#include "SympleCode/Node/Member/ExternFunctionNode.h"
#include "SympleCode/Node/Member/SharedVariableNode.h"
#include "SympleCode/Node/Member/GlobalStatementNode.h"
#include "SympleCode/Node/Member/StructDeclarationNode.h"
#include "SympleCode/Node/Member/FunctionDeclarationNode.h"
#include "SympleCode/Node/Member/GlobalVariableDeclarationNode.h"

#include "SympleCode/Node/Expression/ExpressionNode.h"
#include "SympleCode/Node/Expression/CastExpressionNode.h"
#include "SympleCode/Node/Expression/ListExpressionNode.h"
#include "SympleCode/Node/Expression/StallocExpressionNode.h"
#include "SympleCode/Node/Expression/TernaryExpressionNode.h"
#include "SympleCode/Node/Expression/FunctionCallExpressionNode.h"
#include "SympleCode/Node/Expression/ParenthesizedExpressionNode.h"
#include "SympleCode/Node/Expression/FunctionPointerExpressionNode.h"
#include "SympleCode/Node/Expression/StructInitializerExpressionNode.h"

#include "SympleCode/Node/Expression/Literal/LiteralExpressionNode.h"

#include "SympleCode/Node/Expression/Modifiable/FieldExpressionNode.h"
#include "SympleCode/Node/Expression/Modifiable/PointerIndexExpressionNode.h"
#include "SympleCode/Node/Expression/Modifiable/DereferencePointerExpressionNode.h"

namespace Symple
{
	class Parser
	{
	private:
		Preprocessor mPreprocessor;

		std::vector<const Token*> mTokens;
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
		TypeNode* ParseType(const Type* type = nullptr);
		TypeContinueNode* ParseTypeContinue();

		const std::vector<const MemberNode*> ParseMembers();
		MemberNode* ParseMember();

		FunctionDeclarationNode* ParseFunctionDeclaration();
		FunctionArgumentsNode* ParseFunctionArguments();
		FunctionModifiersNode* ParseFunctionModifiers();
		FunctionModifierNode* ParseFunctionModifier();
		FunctionArgumentNode* ParseFunctionArgument();
		SharedVariableNode* ParseSharedVariable(const Type* type = nullptr);
		ExternFunctionNode* ParseExternFunction();
		FunctionHintNode* ParseFunctionHint();
		GlobalVariableDeclarationNode* ParseGlobalVariableDeclaration(const Type* type = nullptr);
		StructDeclarationNode* ParseStructDeclaration();
		FieldListNode* ParseFieldList();
		VariableDeclarationNode* ParseField(const Type* type = nullptr);

		StatementNode* ParseStatement(bool matchSemicolon = true);
		IfStatementNode* ParseIfStatement(bool matchSemicolon = true);
		AsmStatementNode* ParseAsmStatement(bool matchSemicolon = true);
		BreakStatementNode* ParseBreakStatement(bool matchSemicolon = true);
		WhileStatementNode* ParseWhileStatement(bool matchSemicolon = true);
		BlockStatementNode* ParseBlockStatement(bool matchSemicolon = true);
		ReturnStatementNode* ParseReturnStatement(bool matchSemicolon = true);
		GlobalStatementNode* ParseGlobalStatement(bool matchSemicolon = true);
		ForLoopStatementNode* ParseForLoopStatement(bool matchSemicolon = true);
		GlobalVariableDeclarationNode* ParseEnumDeclaration(bool matchSemicolon = true);
		GlobalVariableDeclarationNode* ParseEnumField(const TypeNode* type, int before = 0);
		VariableDeclarationNode* ParseVariableDeclaration(bool matchSemicolon = true, const Type* type = nullptr);
		VariableModifiersNode* ParseVariableModifiers();
		VariableModifierNode* ParseVariableModifier();
	
		ExpressionNode* ParseExpression();

		ExpressionNode* ParseAssignmentExpression();
		ExpressionNode* ParseUnaryExpression(int parentPriority = -1);
		ExpressionNode* ParseBinaryExpression(int parentPriority = -1);
		ExpressionNode* ParsePrimaryExpression();

		CastExpressionNode* ParseCastExpression();
		ListExpressionNode* ParseListExpression();
		StallocExpressionNode* ParseStallocExpression();
		TernaryExpressionNode* ParseTernaryExpression(ExpressionNode* condition);
		ExpressionNode* ParseParenthesizedExpression();
		FunctionPointerExpressionNode* ParseFunctionPointerExpression();
		StructInitializerExpressionNode* ParseStructInitializerExpression();

		ModifiableExpressionNode* ParseModifiableExpression();
		VariableExpressionNode* ParseVariableExpression();
		FieldExpressionNode* ParseFieldExpression(ModifiableExpressionNode* callee);
		VariableAddressExpressionNode* ParseVariableAddressExpression();
		PointerIndexExpressionNode* ParsePointerIndexExpression(ExpressionNode* address);
		DereferencePointerExpressionNode* ParseDereferencePointerExpression();

		FunctionCallArgumentsNode* ParseFunctionCallArguments();
		FunctionCallExpressionNode* ParseFunctionCallExpression();
	};
}