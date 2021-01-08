#pragma once

#include "SympleCode/Node/CompilationUnitNode.h"

#include "SympleCode/Node/Member/FunctionDeclarationNode.h"

#include "SympleCode/Node/Statement/ReturnStatementNode.h"
#include "SympleCode/Node/Statement/ExpressionStatementNode.h"
#include "SympleCode/Node/Statement/VariableDeclarationNode.h"

#include "SympleCode/Node/Expression/CastExpressionNode.h"
#include "SympleCode/Node/Expression/FunctionCallExpressionNode.h"
#include "SympleCode/Node/Expression/ParenthesizedExpressionNode.h"

#include "SympleCode/Node/Expression/Literal/NullLiteralExpressionNode.h"
#include "SympleCode/Node/Expression/Literal/NumberLiteralExpressionNode.h"
#include "SympleCode/Node/Expression/Literal/StringLiteralExpressionNode.h"
#include "SympleCode/Node/Expression/Literal/BooleanLiteralExpressionNode.h"
#include "SympleCode/Node/Expression/Literal/CharacterLiteralExpressionNode.h"

#include "SympleCode/Node/Expression/Modifiable/FieldExpressionNode.h"
#include "SympleCode/Node/Expression/Modifiable/VariableExpressionNode.h"
#include "SympleCode/Node/Expression/Modifiable/AssignmentExpressionNode.h"
#include "SympleCode/Node/Expression/Modifiable/DereferencePointerExpressionNode.h"

#include "SympleCode/Node/Expression/Operator/UnaryExpressionNode.h"
#include "SympleCode/Node/Expression/Operator/BinaryExpressionNode.h"

#include <cstdio>

namespace Symple
{
	struct Emit
	{
		const ExpressionNode* Node = nullptr;
		const char* Eval = nullptr;
		const unsigned int Size = 4;

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

		const char* mPath;

		unsigned int mData;
		unsigned int mReturn;
		unsigned int mStack;

		bool mReturning;
		bool mReturnStatement;
	public:
		Emitter(const char* path);
		~Emitter();

		Emit EmitCompilationUnit(const CompilationUnitNode* unit);
	private:
		Emit Lea(Emit from, Emit to);
		Emit Push(Emit emit);
		Emit Pop(Emit emit);
		Emit Move(Emit from, Emit to);

		Emit Add(Emit from, Emit to);
		Emit Sub(Emit from, Emit to);
		Emit Xor(Emit from, Emit to);

		Emit Neg(Emit emit);

		Emit EmitMember(const MemberNode* member);
		Emit EmitFunctionDeclaration(const FunctionDeclarationNode* declaration);

		Emit EmitStatement(const StatementNode* statement);
		Emit EmitReturnStatement(const ReturnStatementNode* statement);
		Emit EmitExpressionStatement(const ExpressionStatementNode* statement);
		Emit EmitVariableDeclaration(const VariableDeclarationNode* declaration);

		Emit EmitExpression(const ExpressionNode* expression);
		Emit EmitCastExpression(const CastExpressionNode* expression);
		Emit EmitFunctionCallExpression(const FunctionCallExpressionNode* call);
		Emit EmitParenthesizedExpression(const ParenthesizedExpressionNode* expression);

		Emit EmitLiteralExpression(const LiteralExpressionNode* expression);
		Emit EmitNullLiteralExpression(const NullLiteralExpressionNode* expression);
		Emit EmitNumberLiteralExpression(const NumberLiteralExpressionNode* expression);
		Emit EmitStringLiteralExpression(const StringLiteralExpressionNode* expression);
		Emit EmitBooleanLiteralExpression(const BooleanLiteralExpressionNode* expression);
		Emit EmitCharacterLiteralExpression(const CharacterLiteralExpressionNode* expression);

		Emit EmitModifiableExpression(const ModifiableExpressionNode* expression);
		Emit EmitFieldExpression(const FieldExpressionNode* expression);
		Emit EmitVariableExpression(const VariableExpressionNode* expression);
		Emit EmitAssignmentExpression(const AssignmentExpressionNode* expression);
		Emit EmitDereferencePointerExpression(const DereferencePointerExpressionNode* expression);

		Emit EmitOperatorExpression(const OperatorExpressionNode* expression);
		Emit EmitUnaryExpression(const UnaryExpressionNode* expression);
		Emit EmitBinaryExpression(const BinaryExpressionNode* expression);

		bool OpenFile();
		bool OpenLiteralFile();
	};
}