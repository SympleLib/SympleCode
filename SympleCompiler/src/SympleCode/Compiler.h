#pragma once

#include <vector>
#include <string>

#if SY_32
constexpr int platsize = 4;
#elif SY_64
constexpr int platsize = 8;
#else
#error Unknown Platform!
#endif

namespace Symple
{
	class Compiler
	{
	private:
		std::string mOutput;
		std::vector<std::string> mObjectFiles;
	public:
		bool CompileFile(const std::string& path);
		void Link(const char* output, const std::vector<const char*> libraries = {});
		int Run();
	};
}