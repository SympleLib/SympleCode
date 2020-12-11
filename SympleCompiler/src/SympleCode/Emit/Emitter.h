#pragma once

#include <cstdio>
#include <vector>

#include "SympleCode/Common/Node/CompilationUnitNode.h"
#include "SympleCode/Common/Node/StatementNode.h"
#include "SympleCode/Common/Node/ExpressionNode.h"
#include "SympleCode/Common/Node/BinaryExpressionNode.h"

namespace Symple
{
	class Emitter
	{
	private:
		bool mOpen;
		const char* mPath;
		FILE* mFile;
	public:
		Emitter(const char* path);
		~Emitter();

		void Emit(const CompilationUnitNode* compilationUnit);
	private:
		void EmitMember(const MemberNode* member);
		void EmitStatement(const StatementNode* statement);

		void EmitExpression(const ExpressionNode* expression);
		void EmitBinaryExpression(const BinaryExpressionNode* expression);

		bool OpenFile();
	};
}