#include <windows.h>
#include <iostream>
#include <cstring>

#include "SympleCode/Parser/Parser.h"
#include "SympleCode/Emit/Emitter.h"

bool CompileFile(const char* path, const char* syt, const char* asmS, const char* obj)
{
	FILE* file;
	errno_t err;
	if (!(err = fopen_s(&file, path, "rb")) && file)
	{
		fseek(file, 0L, SEEK_END);
		unsigned int size = min(ftell(file), 4096);
		rewind(file);
		char* source = new char[size + 1];
		fread(source, 1, size, file);
		source[size] = 0;
		fclose(file);

		Symple::Parser parser(source);
		Symple::CompilationUnitNode* tree = parser.ParseCompilationUnit();
		Symple::Diagnostics* diagnostics = parser.GetDiagnostics();

		FILE* treef;
		errno_t err;
		if (!(err = fopen_s(&treef, syt, "w")) && treef)
		{
			fputs(tree->ToString().c_str(), treef);
			fclose(treef);
		}

		unsigned int parseErrors = diagnostics->GetErrors().size();
		if (parseErrors)
		{
			printf("Compiled %s with %i errors, %i warnings (total: %i)\n", path, diagnostics->GetErrors().size(), diagnostics->GetWarnings().size(), diagnostics->GetMessages().size());

			for (const Symple::Message* error : diagnostics->GetErrors())
			{
				std::cout << "[!]<" << error->Token->GetLine() << ':' << error->Token->GetColumn() << ">: " << error->Message << '\n';
			}

			for (const Symple::Message* warning : diagnostics->GetWarnings())
			{
				std::cout << "[?]<" << warning->Token->GetLine() << ':' << warning->Token->GetColumn() << ">: " << warning->Message << '\n';
			}
			printf("No Code Generated :(\n");
		}
		else
		{
			{
				Symple::Emitter emitter(diagnostics, asmS);
				emitter.Emit(tree);

				printf("Compiled %s with %i errors, %i warnings (total: %i)\n", path, diagnostics->GetErrors().size(), diagnostics->GetWarnings().size(), diagnostics->GetMessages().size());

				for (const Symple::Message* error : diagnostics->GetErrors())
				{
					std::cout << "[!]<" << error->Token->GetLine() << ':' << error->Token->GetColumn() << ">: " << error->Message << '\n';
				}

				for (const Symple::Message* warning : diagnostics->GetWarnings())
				{
					std::cout << "[?]<" << warning->Token->GetLine() << ':' << warning->Token->GetColumn() << ">: " << warning->Message << '\n';
				}
			}

			if (!diagnostics->GetErrors().size())
			{
				char command[128];
				sprintf_s(command, "vender\\clang -c %s -o %s", asmS, obj);
				system(command);

				return true;
			}
		}
	}
	else
	{
		char errMsg[32];
		if (!strerror_s(errMsg, err))
			std::cerr << "[!]: Error opening sample file: " << errMsg << "!\n";
		else
			std::cerr << "[!]: Unkown Error opening sample file!\n";
	}

	return false;
}

int main()
{
	//ShowWindow(GetConsoleWindow(), FALSE);
	SetConsoleTitle(L"Symple Compiler - Treidex");

	if (CompileFile("sy/Main.symple", "sy/Main.syt", "sy/Main.s", "sy/Main.obj") &&
		CompileFile("sy/Math.symple", "sy/Math.syt", "sy/Math.s", "sy/Math.obj"))
	{
		system("vender\\clang sy/Main.obj sy/Math.obj -llegacy_stdio_definitions.lib -lgdi32.lib -luser32.lib --optimize -o sy/Sample.exe");

		printf("Executing Program...\n");
		printf("Program Exited with Code %i\n", system("sy\\Sample"));
	}

	std::cin.get();
}