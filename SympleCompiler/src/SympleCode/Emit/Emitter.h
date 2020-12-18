#pragma once

#include <cstdio>
#include <map>

#include "SympleCode/Common/Analysis/Diagnostics.h"

#include "SympleCode/Common/Node/Statement/StatementNode.h"
#include "SympleCode/Common/Node/Statement/ReturnStatementNode.h"

#include "SympleCode/Common/Node/Statement/IfStatementNode.h"
#include "SympleCode/Common/Node/Statement/WhileStatementNode.h"
#include "SympleCode/Common/Node/Statement/BreakStatementNode.h"
#include "SympleCode/Common/Node/Statement/VariableDeclarationNode.h"

#include "SympleCode/Common/Node/Expression/ExpressionNode.h"
#include "SympleCode/Common/Node/Expression/UnaryExpressionNode.h"
#include "SympleCode/Common/Node/Expression/BinaryExpressionNode.h"
#include "SympleCode/Common/Node/Expression/LiteralExpressionNode.h"
#include "SympleCode/Common/Node/Expression/VariableExpressionNode.h"
#include "SympleCode/Common/Node/Expression/FunctionCallExpressionNode.h"
#include "SympleCode/Common/Node/Expression/PointerIndexExpressionNode.h"
#include "SympleCode/Common/Node/Expression/StringLiteralExpressionNode.h"

#include "SympleCode/Common/Node/CompilationUnitNode.h"

#include "SympleCode/Common/Node/Statement/StructDeclarationNode.h"
#include "SympleCode/Common/Node/Member/FunctionDeclarationNode.h"

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

		void EmitCast(int from, int to = 4);

		void EmitMember(const MemberNode* member);

		void EmitFunctionDeclaration(const FunctionDeclarationNode* declaration);
		void EmitVariableDeclaration(const VariableDeclarationNode* declaration);

		void EmitStatement(const StatementNode* statement);
		void EmitIfStatement(const IfStatementNode* statement);
		void EmitBreakStatement(const BreakStatementNode* statement);
		void EmitBlockStatement(const BlockStatementNode* statement);
		void EmitWhileStatement(const WhileStatementNode* statement);

		void EmitExpression(const ExpressionNode* expression, int size = 4);
		void EmitUnaryExpression(const UnaryExpressionNode* expression, int size = 4);
		void EmitBinaryExpression(const BinaryExpressionNode* expression, int size = 4);
		void EmitLiteralExpression(const LiteralExpressionNode* expression, int size = 4);
		void EmitVariableExpression(const VariableExpressionNode* expression, int size = 4);
		void EmitFunctionCallExpression(const FunctionCallExpressionNode* call, int size = 4);
		void EmitPointerIndexExpression(const PointerIndexExpressionNode* expression, int size = 4);
		void EmitStringLiteralExpression(const StringLiteralExpressionNode* expression, int size = 4);

		bool VariableDefined(const std::string_view& name);

		bool OpenFile();
		bool OpenLiteralFile();
	};
}