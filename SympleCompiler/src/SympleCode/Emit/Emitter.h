#pragma once

#include <map>
#include <sstream>
#include <iostream>

#include "SympleCode/Common/Analysis/Diagnostics.h"

#include "SympleCode/Common/Node/CompilationUnitNode.h"

#include "SympleCode/Common/Node/Member/GlobalStatementNode.h"
#include "SympleCode/Common/Node/Member/FunctionDeclarationNode.h"

#include "SympleCode/Common/Node/Statement/IfStatementNode.h"
#include "SympleCode/Common/Node/Statement/BreakStatementNode.h"
#include "SympleCode/Common/Node/Statement/WhileStatementNode.h"
#include "SympleCode/Common/Node/Statement/ReturnStatementNode.h"
#include "SympleCode/Common/Node/Statement/ExpressionStatementNode.h"

#include "SympleCode/Common/Node/Expression/Operator/UnaryExpressionNode.h"
#include "SympleCode/Common/Node/Expression/Operator/BinaryExpressionNode.h"
#include "SympleCode/Common/Node/Expression/Operator/AssignmentExpressionNode.h"

#include "SympleCode/Common/Node/Expression/Literal/LiteralExpressionNode.h"
#include "SympleCode/Common/Node/Expression/Literal/StringLiteralExpressionNode.h"

#include "SympleCode/Common/Node/Expression/VariableExpressionNode.h"
#include "SympleCode/Common/Node/Expression/FunctionCallExpressionNode.h"
#include "SympleCode/Common/Node/Expression/PointerIndexExpressionNode.h"

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

		std::vector<unsigned int> mBreakPoints;

		std::map<std::string_view, const VariableDeclarationNode*> mDeclaredVariables;
	public:
		Emitter(Diagnostics* diagnostics, const char* path);
		~Emitter();

		void Emit(const CompilationUnitNode* unit);
	private:
		static char  Rep(int size = 4);
		static char* RegAx(int size = 4);
		static char* RegDx(int size = 4);

		static char* Format(char* fmt, ...);

		char* Push(char* reg = RegAx());
		char* Pop(char* reg = RegAx());

		char* Move(char* from = RegAx(), char* to = RegAx(), int size = 4);
		char* Cast(char* reg = RegAx(), int from = 4, bool toRegAx = true, int to = 4);
		char* Lea(char* from = RegAx(), char* to = RegAx());

		char* Add(char* right = RegDx(), char* left = RegAx());
		char* Sub(char* right = RegDx(), char* left = RegAx());
		char* Mul(char* right = RegDx(), char* left = RegAx());
		char* Div(char* left = RegAx());
		char* Mod(char* left = RegAx());

		char* And(char* right = RegDx(), char* left = RegDx(), int size = 4);
		char* Or(char* right = RegDx(), char* left = RegDx(), int size = 4);
		char* Xor(char* right = RegDx(), char* left = RegDx(), int size = 4);
		char* Neg(char* val = RegAx());

		char* Cmp(char* right = RegDx(), char* left = RegDx(), int size = 4);
		char* CmpOp(const Token* oqerator);
		char* CmpNOp(const Token* oqerator);

		char* EmitMember(const MemberNode* member);

		char* EmitFunctionDeclaration(const FunctionDeclarationNode* declaration);

		char* EmitStatement(const StatementNode* statement);
		char* EmitBlockStatement(const BlockStatementNode* body);
		char* EmitIfStatement(const IfStatementNode* declaration);
		char* EmitBreakStatement(const BreakStatementNode* statement);
		char* EmitWhileStatement(const WhileStatementNode* declaration);
		char* EmitReturnStatement(const ReturnStatementNode* statement);
		char* EmitExpressionStatement(const ExpressionStatementNode* statement);
		char* EmitVariableDeclaration(const VariableDeclarationNode* declaration);

		char* EmitExpression(const ExpressionNode* expression);
		char* EmitUnaryExpression(const UnaryExpressionNode* expression);
		char* EmitBinaryExpression(const BinaryExpressionNode* expression);
		char* EmitLiteralExpression(const LiteralExpressionNode* expression);
		char* EmitVariableExpression(const VariableExpressionNode* expression);
		char* EmitFunctionCallExpression(const FunctionCallExpressionNode* call);
		char* EmitPointerIndexExpression(const PointerIndexExpressionNode* expression);
		char* EmitStringLiteralExpression(const StringLiteralExpressionNode* expression);
		char* EmitAssignmentExpression(const AssignmentExpressionNode* expression);

		struct ModifiableExpression
		{
			char* Emit;
			int Size;
		} EmitModifiableExpression(const ModifiableExpressionNode* expression);
		ModifiableExpression EmitModifiableVariableExpression(const VariableExpressionNode* expression);

		char* PopToRegMod();

		ModifiableExpression EmitModifiableAssignmentExpression(const AssignmentExpressionNode* expression);
		ModifiableExpression EmitModifiablePointerIndexExpression(const PointerIndexExpressionNode* expression);

		bool VariableDefined(const std::string_view& name);

		bool OpenFile();
		bool OpenLiteralFile();
	};
}