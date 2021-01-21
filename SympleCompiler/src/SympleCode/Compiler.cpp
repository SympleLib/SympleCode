#include "SympleCode/Compiler.h"

#include "SympleCode/Parser/Parser.h"
#include "SympleCode/Emit/Emitter.h"

#include "SympleCode/Analysis/Debug.h"
#include "SympleCode/Analysis/Diagnostics.h"

#include <algorithm>
#include <iostream>
#include <sstream>

#define NOMINMAX
#include <Windows.h>

namespace Symple
{
	Compiler::Compiler(const std::string& include)
		: mInclude(include) {}

	bool Compiler::CompileFile(const std::string& pathStr)
	{
		Debug::Clear();
		Diagnostics::Clear();

		std::string odir = "bin\\" + pathStr.substr(0, pathStr.find_last_of('\\'));
		std::string cmd = "mkdir " + odir;
		if (GetFileAttributesA(odir.c_str()) == INVALID_FILE_ATTRIBUTES)
			system(cmd.c_str());

		std::string sytStr = "bin\\" + pathStr.substr(0, pathStr.find_last_of('.')) + ".syt";
		std::string asmSStr = "bin\\" + pathStr.substr(0, pathStr.find_last_of('.')) + ".S";
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
			Parser parser(source, path, mInclude.c_str());
			unsigned int parseErrors = Diagnostics::GetErrors().size();
			if (parseErrors)
			{
				printf("Lexed/Preprocessed %s with %i errors, %i warnings (total: %i)\n", path, Diagnostics::GetErrors().size(), Diagnostics::GetWarnings().size(), Diagnostics::GetMessages().size());

				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_INTENSITY);
				for (const Message* error : Diagnostics::GetErrors())
					std::cout << "[!](" << error->Token->GetFile() << ")<" << error->Token->GetLine() << ':' << error->Token->GetColumn() << ">: " << error->Message << '\n';

				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
				for (const Message* warning : Diagnostics::GetWarnings())
					std::cout << "[?](" << warning->Token->GetFile() << ")<" << warning->Token->GetLine() << ':' << warning->Token->GetColumn() << ">: " << warning->Message << '\n';
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
			}
			else
			{
				CompilationUnitNode* unit = parser.ParseCompilationUnit();

				parseErrors = Diagnostics::GetErrors().size();
				if (parseErrors)
				{
					printf("Parsed %s with %i errors, %i warnings (total: %i)\n", path, Diagnostics::GetErrors().size(), Diagnostics::GetWarnings().size(), Diagnostics::GetMessages().size());

					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_INTENSITY);
					for (const Message* error : Diagnostics::GetErrors())
						std::cout << "[!](" << error->Token->GetFile() << ")<" << error->Token->GetLine() << ':' << error->Token->GetColumn() << ">: " << error->Message << '\n';

					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
					for (const Message* warning : Diagnostics::GetWarnings())
						std::cout << "[?](" << warning->Token->GetFile() << ")<" << warning->Token->GetLine() << ':' << warning->Token->GetColumn() << ">: " << warning->Message << '\n';
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
				}
				else
				{
					FILE* treef;
					errno_t err;
					if (!(err = fopen_s(&treef, syt, "w")) && treef)
					{
						fputs(unit->ToString().c_str(), treef);
						fclose(treef);
					}
					else
					{
						char errMsg[32];
						if (!strerror_s(errMsg, err))
							std::cerr << "[!]: Error opening file '" << syt << "': " << errMsg << "!\n";
						else
							std::cerr << "[!]: Unkown Error opening file '" << syt << "'!\n";
					}

					{
						printf("Generating...\n");

						Emitter emitter(asmS);
						emitter.EmitCompilationUnit(unit);

						printf("Compiled %s with %i errors, %i warnings (total: %i)\n", path, Diagnostics::GetErrors().size(), Diagnostics::GetWarnings().size(), Diagnostics::GetMessages().size());

						SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_INTENSITY);
						for (const Message* error : Diagnostics::GetErrors())
							std::cout << "[!](" << error->Token->GetFile() << ")<" << error->Token->GetLine() << ':' << error->Token->GetColumn() << ">: " << error->Message << '\n';

						SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
						for (const Message* warning : Diagnostics::GetWarnings())
							std::cout << "[?](" << warning->Token->GetFile() << ")<" << warning->Token->GetLine() << ':' << warning->Token->GetColumn() << ">: " << warning->Message << '\n';
						SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
					}

					if (!Diagnostics::GetErrors().size())
						//return true;
					{
						char command[128];
#if SY_32
						sprintf_s(command, "clang_x86 -c %s -o %s --optimize", asmS, obj);
#else
						sprintf_s(command, "clang_x64 -c %s -o %s --optimize", asmS, obj);
#endif
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

		printf("No Code Generated :(\n");

		return false;
	}

	void Compiler::Link(const char* output, const std::vector<const char*> libraries)
	{
		// Static Initialization
		{
			{
				Emitter emitter("bin\\__staticinit.S");
				emitter.EmitStaticInitialization();
			}

			char command[128];
#if SY_32
			sprintf_s(command, "clang_x86 -c %s -o %s", "bin\\__staticinit.S", "bin\\__staticinit.o --optimize");
#else
			sprintf_s(command, "clang_x64 -c %s -o %s", "bin\\__staticinit.S", "bin\\__staticinit.o --optimize");
#endif
			int compileStatis = system(command);

			mObjectFiles.push_back("bin\\__staticinit.o");
		}

		mOutput = output;
		
		std::stringstream ss;
#if SY_32
		ss << "clang_x86";
#else
		ss << "clang_x64";
#endif
		ss << " -o " << output << " --optimize";
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