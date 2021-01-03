#include "SympleCode/Compiler.h"

#include "SympleCode/Parser/Parser.h"

#include "SympleCode/Common/Analysis/Debug.h"
#include "SympleCode/Common/Analysis/Diagnostics.h"

#include <algorithm>
#include <iostream>
#include <sstream>

#define NOMINMAX
#include <Windows.h>

namespace Symple
{
	bool Compiler::CompileFile(const std::string& pathStr)
	{
		Debug::Clear();
		Diagnostics::Clear();

		std::string dir = "bin\\" + pathStr.substr(0, pathStr.find_last_of('\\'));
		std::string cmd = "mkdir " + dir;
		if (GetFileAttributesA(dir.c_str()) == INVALID_FILE_ATTRIBUTES)
			system(cmd.c_str());

		std::string sytStr = "bin\\" + pathStr.substr(0, pathStr.find_last_of('.')) + ".syt";
		std::string asmSStr = "bin\\" + pathStr.substr(0, pathStr.find_last_of('.')) + ".s";
		std::string objStr = "bin\\" + pathStr.substr(0, pathStr.find_last_of('.')) + ".o";

		const char* path = pathStr.c_str();
		const char* syt = sytStr.c_str();
		const char* asmS = asmSStr.c_str();
		const char* obj = objStr.c_str();

		FILE* file;
		errno_t err;
		if (!(err = fopen_s(&file, path, "rb")) && file)
		{
			fseek(file, 0L, SEEK_END);
			unsigned int size = std::min(ftell(file), 4096L);
			rewind(file);
			char* source = new char[size + 1];
			fread(source, 1, size, file);
			source[size] = 0;
			fclose(file);

			printf("Parsing...\n");
			Parser parser(source, path);
			CompilationUnitNode* tree = parser.ParseCompilationUnit();

			//FILE* treef;
			//errno_t err;
			//if (!(err = fopen_s(&treef, syt, "w")) && treef)
			//{
			//	fputs(tree->ToString().c_str(), treef);
			//	fclose(treef);
			//}
			//else
			//{
			//	char errMsg[32];
			//	if (!strerror_s(errMsg, err))
			//		std::cerr << "[!]: Error opening file '" << syt << "': " << errMsg << "!\n";
			//	else
			//		std::cerr << "[!]: Unkown Error opening file '" << syt << "'!\n";
			//}

			unsigned int parseErrors = Diagnostics::GetErrors().size();
			if (parseErrors)
			{
				printf("Compiled %s with %i errors, %i warnings (total: %i)\n", path, Diagnostics::GetErrors().size(), Diagnostics::GetWarnings().size(), Diagnostics::GetMessages().size());

				for (const Message* error : Diagnostics::GetErrors())
				{
					std::cout << "[!](" << error->Token->GetFile() << ")<" << error->Token->GetLine() << ':' << error->Token->GetColumn() << ">: " << error->Message << '\n';
				}

				for (const Message* warning : Diagnostics::GetWarnings())
				{
					std::cout << "[?](" << warning->Token->GetFile() << ")<" << warning->Token->GetLine() << ':' << warning->Token->GetColumn() << ">: " << warning->Message << '\n';
				}
			}
			else
			{
				{
					printf("Generating...\n");
					

					printf("Compiled %s with %i errors, %i warnings (total: %i)\n", path, Diagnostics::GetErrors().size(), Diagnostics::GetWarnings().size(), Diagnostics::GetMessages().size());

					for (const Message* error : Diagnostics::GetErrors())
					{
						std::cout << "[!](" << error->Token->GetFile() << ")<" << error->Token->GetLine() << ':' << error->Token->GetColumn() << ">: " << error->Message << '\n';
					}

					for (const Message* warning : Diagnostics::GetWarnings())
					{
						std::cout << "[?](" << warning->Token->GetFile() << ")<" << warning->Token->GetLine() << ':' << warning->Token->GetColumn() << ">: " << warning->Message << '\n';
					}
				}

				if (!Diagnostics::GetErrors().size())
				{
					char command[128];
					sprintf_s(command, "as -c %s -o %s", asmS, obj);
					int compileStatis = system(command);

					mObjectFiles.push_back(objStr);

					return !compileStatis;
				}
			}
			printf("No Code Generated :(\n");

			return false;
		}
		char errMsg[32];
		if (!strerror_s(errMsg, err))
			std::cerr << "[!]: Error opening file '" << path << "': " << errMsg << "!\n";
		else
			std::cerr << "[!]: Unkown Error opening file '" << path << "'!\n";

		return false;
	}

	void Compiler::Link(const char* output, const std::vector<const char*> libraries)
	{
		mOutput = output;
		
		std::stringstream ss;
		ss << "clang -o " << output << " --optimize=3";
		for (const std::string& objFile : mObjectFiles)
			ss << ' ' << objFile;
		for (const char* library : libraries)
			ss << " -l " << library;

		puts("Linking Program...");
		system(ss.str().c_str());
	}

	int Compiler::Run()
	{
		puts("Running Program...");
		int exitCode = system(mOutput.c_str());
		printf("\nProgram Exited with Code %i (0x%x)\n", exitCode, exitCode);

		return exitCode;
	}
}