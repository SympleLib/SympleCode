#pragma once

#include <cstdio>
#include <vector>

#include "SympleCode/Common/Analysis/Diagnostics.h"

#include "SympleCode/Common/Node/StatementNode.h"
#include "SympleCode/Common/Node/ReturnStatementNode.h"

#include "SympleCode/Common/Node/ExpressionNode.h"
#include "SympleCode/Common/Node/CompilationUnitNode.h"
#include "SympleCode/Common/Node/BinaryExpressionNode.h"
#include "SympleCode/Common/Node/LiteralExpressionNode.h"
#include "SympleCode/Common/Node/FunctionDeclarationNode.h"
#include "SympleCode/Common/Node/FunctionCallExpressionNode.h"
#include "SympleCode/Common/Node/VariableDeclarationNode.h"
#include "SympleCode/Common/Node/VariableExpressionNode.h"

#define DO_COMMENTS false

namespace Symple
{
	class Emitter
	{
	private:
		bool mOpen;
		const char* mPath;
		FILE* mFile;
		Diagnostics* mDiagnostics;

		unsigned int mStackPos;
	public:
		Emitter(Diagnostics* diagnostics, const char* path);
		~Emitter();

		void Emit(const CompilationUnitNode* compilationUnit);
	private:
		void EmitMember(const MemberNode* member);

		void EmitFunctionDeclaration(const FunctionDeclarationNode* declaration);
		void EmitVariableDeclaration(const VariableDeclarationNode* declaration);

		void EmitStatement(const StatementNode* statement);

		void EmitExpression(const ExpressionNode* expression);
		void EmitBinaryExpression(const BinaryExpressionNode* expression);
		void EmitLiteralExpression(const LiteralExpressionNode* expression);
		void EmitVariableExpression(const VariableExpressionNode* expression);
		void EmitFunctionCallExpression(const FunctionCallExpressionNode* call);

		bool OpenFile();
	};
}