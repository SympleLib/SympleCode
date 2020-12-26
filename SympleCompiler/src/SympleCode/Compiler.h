#pragma once

#include <vector>
#include <string>

namespace Symple
{
	class Compiler
	{
	private:
		std::string mOutput;
		std::vector<std::string> mObjectFiles;
	public:
		bool CompileFile(const std::string& path);
		void Link(const char* output, const std::vector<const char*> libraries = { "legacy_stdio_definitions.lib", "user32.lib" });
		int Run();
	};
}