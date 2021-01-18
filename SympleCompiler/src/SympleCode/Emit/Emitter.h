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
#include "SympleCode/Node/Expression/StallocExpressionNode.h"
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

#define nullreg -1
#define regbp -2
#define regsp -3
#define regax 0
#define regdx 1
#define regcx 2
#define regbx 3

#define regdi 4
#define regsi 5
#define regr8 6
#define regr9 7
#define regr10 8
#define regr11 9
#define regr12 10
#define regr13 11

namespace Symple
{
	typedef int Register;

	class Emitter
	{
	private:
#if SY_32
		static const int NumRegisters = 4;

		bool mFreeRegisters[NumRegisters] = { true, true, true, true };
#else
		static const int NumRegisters = 12;
		static const char* const sRegisters64[NumRegisters];

		bool mFreeRegisters[NumRegisters] = { true, true, true, true, true, true,
			true, true, true, true, true, true };
#endif
		static const char* const sRegisters32[NumRegisters];
		static const char* const sRegisters16[NumRegisters];
		static const char* const sRegisters8[NumRegisters];


		FILE* mFile;
		FILE* mResourceFile;

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
		Register AllocReg(Register = nullreg);
		void FreeReg(Register = nullreg);
		void FreeAllRegs();
		const char* GetReg(Register, int size = platsize);

		char Suf(int size = platsize);
		const char* Word(int size = platsize);

		void Push(Register);
		void Pop(Register);

		int Align(int n, int align);
		void PushStruct(const StructInitializerExpressionNode* ztruct);
		void PushStruct(const StructDeclarationNode* ty, Register ptr);

		void Loc(const Token*);

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
		Register EmitStallocExpression(const StallocExpressionNode*);
		Register EmitTernaryExpression(const TernaryExpressionNode*);
		Register EmitFunctionCallExpression(const FunctionCallExpressionNode*);
		Register EmitParenthesizedExpression(const ParenthesizedExpressionNode*);
		Register EmitVariableAddressExpression(const VariableAddressExpressionNode*);

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
		bool OpenResourceFile();
	};
}