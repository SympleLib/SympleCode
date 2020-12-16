#pragma once

#include <cstdio>
#include <map>

#include "SympleCode/Common/Analysis/Diagnostics.h"

#include "SympleCode/Common/Node/StatementNode.h"
#include "SympleCode/Common/Node/ReturnStatementNode.h"

#include "SympleCode/Common/Node/ExpressionNode.h"
#include "SympleCode/Common/Node/WhileStatementNode.h"
#include "SympleCode/Common/Node/BreakStatementNode.h"
#include "SympleCode/Common/Node/CompilationUnitNode.h"
#include "SympleCode/Common/Node/BinaryExpressionNode.h"
#include "SympleCode/Common/Node/LiteralExpressionNode.h"
#include "SympleCode/Common/Node/VariableExpressionNode.h"
#include "SympleCode/Common/Node/FunctionDeclarationNode.h"
#include "SympleCode/Common/Node/VariableDeclarationNode.h"
#include "SympleCode/Common/Node/FunctionCallExpressionNode.h"
#include "SympleCode/Common/Node/StringLiteralExpressionNode.h"

#define DO_COMMENTS false

namespace Symple
{
	class Emitter
	{
	private:
		const char* mPath;
		FILE* mFile;
		FILE* mLiteralFile;
		Diagnostics* mDiagnostics;

		unsigned int mStackPos;
		unsigned int mStringPos;
		unsigned int mJumpPos;

		std::map<std::string_view, const VariableDeclarationNode*> mDeclaredVariables;
	public:
		Emitter(Diagnostics* diagnostics, const char* path);
		~Emitter();

		void Emit(const CompilationUnitNode* compilationUnit);
	private:
		static char* RegAx(int size = 4);
		static char* RegDx(int size = 4);
		static char Mod(int size = 4);

		void EmitMember(const MemberNode* member);

		void EmitFunctionDeclaration(const FunctionDeclarationNode* declaration);
		void EmitVariableDeclaration(const VariableDeclarationNode* declaration);

		void EmitStatement(const StatementNode* statement);
		void EmitBreakStatement(const BreakStatementNode* statement);
		void EmitBlockStatement(const BlockStatementNode* statement);
		void EmitWhileStatement(const WhileStatementNode* statement);

		void EmitExpression(const ExpressionNode* expression, int size = 4);
		void EmitBinaryExpression(const BinaryExpressionNode* expression, int size = 4);
		void EmitLiteralExpression(const LiteralExpressionNode* expression, int size = 4);
		void EmitVariableExpression(const VariableExpressionNode* expression, int size = 4);
		void EmitFunctionCallExpression(const FunctionCallExpressionNode* call, int size = 4);
		void EmitStringLiteralExpression(const StringLiteralExpressionNode* expression, int size = 4);

		bool VariableDefined(const std::string_view& name);

		bool OpenFile();
		bool OpenLiteralFile();
	};
}