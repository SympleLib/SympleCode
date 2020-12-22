#pragma once

#include <map>
#include <sstream>
#include <iostream>

#include "SympleCode/Common/Analysis/Diagnostics.h"

#include "SympleCode/Common/Node/CompilationUnitNode.h"

#include "SympleCode/Common/Node/Member/GlobalStatementNode.h"
#include "SympleCode/Common/Node/Member/FunctionDeclarationNode.h"

#include "SympleCode/Common/Node/Statement/ReturnStatementNode.h"
#include "SympleCode/Common/Node/Statement/ExpressionStatementNode.h"

#include "SympleCode/Common/Node/Expression/UnaryExpressionNode.h"
#include "SympleCode/Common/Node/Expression/BinaryExpressionNode.h"
#include "SympleCode/Common/Node/Expression/LiteralExpressionNode.h"
#include "SympleCode/Common/Node/Expression/VariableExpressionNode.h"
#include "SympleCode/Common/Node/Expression/AssignmentExpressionNode.h"
#include "SympleCode/Common/Node/Expression/FunctionCallExpressionNode.h"
#include "SympleCode/Common/Node/Expression/StringLiteralExpressionNode.h"

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
		unsigned int mDataPos;

		unsigned int mReturnPos;

		std::map<std::string_view, const VariableDeclarationNode*> mDeclaredVariables;
	public:
		Emitter(Diagnostics* diagnostics, const char* path);
		~Emitter();

		void Emit(const CompilationUnitNode* unit);
	private:
		static char  Mod(int size = 4);
		static char* RegAx(int size = 4);
		static char* RegDx(int size = 4);

		static char* Format(char* fmt, ...);

		char* Push(char* reg = RegAx());
		char* Pop(char* reg = RegAx());

		char* Cast(char* reg = RegAx(), int from = 4, int to = 4);

		char* EmitMember(const MemberNode* member);

		char* EmitFunctionDeclaration(const FunctionDeclarationNode* declaration);

		char* EmitStatement(const StatementNode* statement);
		char* EmitReturnStatement(const ReturnStatementNode* statement);
		char* EmitExpressionStatement(const ExpressionStatementNode* statement);
		char* EmitVariableDeclaration(const VariableDeclarationNode* declaration);

		char* EmitExpression(const ExpressionNode* expression);
		char* EmitUnaryExpression(const UnaryExpressionNode* expression);
		char* EmitBinaryExpression(const BinaryExpressionNode* expression);
		char* EmitLiteralExpression(const LiteralExpressionNode* expression);
		char* EmitVariableExpression(const VariableExpressionNode* expression, bool set = false);
		char* EmitFunctionCallExpression(const FunctionCallExpressionNode* call);
		char* EmitStringLiteralExpression(const StringLiteralExpressionNode* call);
		char* EmitModifiableExpression(const ModifiableExpressionNode* expression);
		char* EmitAssignmentExpression(const AssignmentExpressionNode* expression);

		bool VariableDefined(const std::string_view& name);

		bool OpenFile();
		bool OpenLiteralFile();
	};
}