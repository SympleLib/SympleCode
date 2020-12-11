#pragma once

#include <cstdio>

#include "SympleCode/Common/Node/CompilationUnitNode.h"

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
		bool OpenFile();
	};
}