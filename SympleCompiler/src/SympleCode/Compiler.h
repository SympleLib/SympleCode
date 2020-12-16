#pragma once

#include <vector>
#include <string>

#include "SympleCode/Preprocessor/Preprocessor.h"

namespace Symple
{
	class Compiler
	{
	private:
		std::string mOutput;
		std::vector<std::string> mObjectFiles;

		Preprocessor mPreprocessor;
	public:
		bool CompileFile(const std::string& path);
		void Link(const char* output, const std::vector<const char*> libraries = { "legacy_stdio_definitions.lib" });
		int Run();
	};
}