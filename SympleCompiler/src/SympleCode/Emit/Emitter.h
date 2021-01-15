#pragma once

#include "SympleCode/Node/CompilationUnitNode.h"

#include "SympleCode/Node/Member/SharedVariableNode.h"
#include "SympleCode/Node/Member/GlobalStatementNode.h"
#include "SympleCode/Node/Member/FunctionDeclarationNode.h"
#include "SympleCode/Node/Member/GlobalVariableDeclarationNode.h"

#include "SympleCode/Node/Statement/IfStatementNode.h"
#include "SympleCode/Node/Statement/AsmStatementNode.h"
#include "SympleCode/Node/Statement/BlockStatementNode.h"
#include "SympleCode/Node/Statement/BreakStatementNode.h"
#include "SympleCode/Node/Statement/WhileStatementNode.h"
#include "SympleCode/Node/Statement/ReturnStatementNode.h"
#include "SympleCode/Node/Statement/ForLoopStatementNode.h"
#include "SympleCode/Node/Statement/ExpressionStatementNode.h"
#include "SympleCode/Node/Statement/VariableDeclarationNode.h"

#include "SympleCode/Node/Expression/CastExpressionNode.h"
#include "SympleCode/Node/Expression/ListExpressionNode.h"
#include "SympleCode/Node/Expression/TernaryExpressionNode.h"
#include "SympleCode/Node/Expression/FunctionCallExpressionNode.h"
#include "SympleCode/Node/Expression/ParenthesizedExpressionNode.h"
#include "SympleCode/Node/Expression/StructInitializerExpressionNode.h"

#include "SympleCode/Node/Expression/Literal/NullLiteralExpressionNode.h"
#include "SympleCode/Node/Expression/Literal/NumberLiteralExpressionNode.h"
#include "SympleCode/Node/Expression/Literal/StringLiteralExpressionNode.h"
#include "SympleCode/Node/Expression/Literal/BooleanLiteralExpressionNode.h"
#include "SympleCode/Node/Expression/Literal/CharacterLiteralExpressionNode.h"

#include "SympleCode/Node/Expression/Modifiable/FieldExpressionNode.h"
#include "SympleCode/Node/Expression/Modifiable/VariableExpressionNode.h"
#include "SympleCode/Node/Expression/Modifiable/AssignmentExpressionNode.h"
#include "SympleCode/Node/Expression/Modifiable/PointerIndexExpressionNode.h"
#include "SympleCode/Node/Expression/Modifiable/DereferencePointerExpressionNode.h"

#include "SympleCode/Node/Expression/Operator/UnaryExpressionNode.h"
#include "SympleCode/Node/Expression/Operator/BinaryExpressionNode.h"

#include "SympleCode/Compiler.h"

#include <cstdio>

namespace Symple
{
	struct Register;

	class Emitter
	{
	private:
		friend class RegisterManager;

		RegisterManager* mRegisterManager;

		FILE* mFile;
		FILE* mLiteralFile;

		const char* mPath;

		unsigned int mData;
		unsigned int mReturn;
		unsigned int mBreak;
		unsigned int mStack;

		static unsigned int sInits;

		bool mReturning;
		bool mReturnStatement;
	public:
		Emitter(const char* path);
		~Emitter();

		void EmitCompilationUnit(const CompilationUnitNode*);
		void EmitStaticInitialization();
	private:
		char Suf(int sz = platsize);
		const char* Word(int sz = platsize);

		void Push(Register);
		void Pop(Register);

		void EmitMember(const MemberNode*);
		void EmitGlobalStatement(const GlobalStatementNode*);
		void EmitFunctionDeclaration(const FunctionDeclarationNode*);
		void EmitGlobalVariableDeclaration(const GlobalVariableDeclarationNode*);

		void EmitStatement(const StatementNode*);
		void EmitIfStatement(const IfStatementNode*);
		void EmitAsmStatement(const AsmStatementNode*);
		void EmitBlockStatement(const BlockStatementNode*, bool funcdecl = false);
		void EmitBreakStatement(const BreakStatementNode*);
		void EmitWhileStatement(const WhileStatementNode*);
		void EmitReturnStatement(const ReturnStatementNode*);
		void EmitForLoopStatement(const ForLoopStatementNode*);
		void EmitExpressionStatement(const ExpressionStatementNode*);
		void EmitVariableDeclaration(const VariableDeclarationNode*);

		Register EmitExpression(const ExpressionNode*);
		Register EmitCastExpression(const CastExpressionNode*);
		Register EmitListExpression(const ListExpressionNode*);
		Register EmitTernaryExpression(const TernaryExpressionNode*);
		Register EmitFunctionCallExpression(const FunctionCallExpressionNode*);
		Register EmitParenthesizedExpression(const ParenthesizedExpressionNode*);
		Register EmitVariableAddressExpression(const VariableAddressExpressionNode*);
		void EmitStructInitializerExpression(const StructInitializerExpressionNode*, Register);
		void EmitStructInitializerExpression(const StructInitializerExpressionNode*, const ModifiableExpressionNode*);

		Register EmitModifiableExpression(const ModifiableExpressionNode*, bool retptr = false);
		Register EmitFieldExpression(const FieldExpressionNode*, bool retptr = false);
		Register EmitVariableExpression(const VariableExpressionNode*, bool retptr = false);
		Register EmitAssignmentExpression(const AssignmentExpressionNode*, bool retptr = false);
		Register EmitPointerIndexExpression(const PointerIndexExpressionNode*, bool retptr = false);
		Register EmitDereferencePointerExpression(const DereferencePointerExpressionNode*, bool retptr = false);

		Register EmitOperatorExpression(const OperatorExpressionNode*);
		Register EmitUnaryExpression(const UnaryExpressionNode*);
		Register EmitBinaryExpression(const BinaryExpressionNode*);

		Register EmitLiteralExpression(const LiteralExpressionNode*);
		Register EmitNullLiteralExpression(const NullLiteralExpressionNode*);
		Register EmitNumberLiteralExpression(const NumberLiteralExpressionNode*);
		Register EmitStringLiteralExpression(const StringLiteralExpressionNode*);
		Register EmitBooleanLiteralExpression(const BooleanLiteralExpressionNode*);
		Register EmitCharacterLiteralExpression(const CharacterLiteralExpressionNode*);

		bool OpenFile();
		bool OpenLiteralFile();
	};
}

#include "SympleCode/Emit/RegisterManager.h"