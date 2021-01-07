#pragma once

#include "SympleCode/Node/CompilationUnitNode.h"

#include "SympleCode/Node/Member/FunctionDeclarationNode.h"

#include "SympleCode/Node/Statement/ReturnStatementNode.h"
#include "SympleCode/Node/Statement/ExpressionStatementNode.h"

#include "SympleCode/Node/Expression/Literal/NumberLiteralExpressionNode.h"

#include <cstdio>

namespace Symple
{
	struct Emit
	{
		const Node* Node;
		const char* Eval;
	};

	class Emitter
	{
	private:
		FILE* mFile;

		unsigned int mData;
		unsigned int mReturn;

		bool mReturning;
	public:
		Emitter();

		Emit EmitCompilationUnit(const CompilationUnitNode* unit);
	private:
		Emit Push(Emit emit);
		Emit Pop(Emit emit);
		Emit Move(Emit from, Emit to);

		Emit EmitMember(const MemberNode* member);
		Emit EmitFunctionDeclaration(const FunctionDeclarationNode* declaration);

		Emit EmitStatement(const StatementNode* statement);
		Emit EmitReturnStatement(const ReturnStatementNode* statement);
		Emit EmitExpressionStatement(const ExpressionStatementNode* statement);

		Emit EmitExpression(const ExpressionNode* expression);
		Emit EmitLiteralExpression(const LiteralExpressionNode* expression);
		Emit EmitNumberLiteralExpression(const NumberLiteralExpressionNode* expression);

		bool OpenFile();
	};
}