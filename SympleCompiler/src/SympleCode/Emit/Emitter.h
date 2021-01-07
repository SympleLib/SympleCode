#pragma once

#include "SympleCode/Node/CompilationUnitNode.h"

#include "SympleCode/Node/Member/FunctionDeclarationNode.h"

#include "SympleCode/Node/Statement/ReturnStatementNode.h"
#include "SympleCode/Node/Statement/ExpressionStatementNode.h"

#include "SympleCode/Node/Expression/Literal/NullLiteralExpressionNode.h"
#include "SympleCode/Node/Expression/Literal/NumberLiteralExpressionNode.h"
#include "SympleCode/Node/Expression/Literal/StringLiteralExpressionNode.h"
#include "SympleCode/Node/Expression/Literal/BooleanLiteralExpressionNode.h"
#include "SympleCode/Node/Expression/Literal/CharacterLiteralExpressionNode.h"

#include <cstdio>

namespace Symple
{
	struct Emit
	{
		const ExpressionNode* Node = nullptr;
		const char* Eval = nullptr;
		const int Size = 4;

		bool operator ==(const Emit& other) const
		{
			return Eval == other.Eval && Size == other.Size;
		}

		bool operator !=(const Emit& other) const
		{
			return !operator==(other);
		}
	};

	class Emitter
	{
	private:
		FILE* mFile;
		FILE* mLiteralFile;

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
		Emit EmitNullLiteralExpression(const NullLiteralExpressionNode* expression);
		Emit EmitNumberLiteralExpression(const NumberLiteralExpressionNode* expression);
		Emit EmitStringLiteralExpression(const StringLiteralExpressionNode* expression);
		Emit EmitBooleanLiteralExpression(const BooleanLiteralExpressionNode* expression);
		Emit EmitCharacterLiteralExpression(const CharacterLiteralExpressionNode* expression);

		bool OpenFile();
		bool OpenLiteralFile();
	};
}