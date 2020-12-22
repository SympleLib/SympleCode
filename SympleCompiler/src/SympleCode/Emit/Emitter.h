#pragma once

#include <cstdio>
#include <map>
#include <sstream>

#include "SympleCode/Common/Analysis/Diagnostics.h"

#include "SympleCode/Common/Node/CompilationUnitNode.h"
#include "SympleCode/Common/Node/Member/FunctionDeclarationNode.h"

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

		char* EmitMember(const MemberNode* member);

		char* EmitFunctionDeclaration(const FunctionDeclarationNode* declaration);

		bool OpenFile();
		bool OpenLiteralFile();
	};
}