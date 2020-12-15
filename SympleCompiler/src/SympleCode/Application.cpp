#include <windows.h>
#include <iostream>
#include <cstring>

#include "SympleCode/Parser/Parser.h"
#include "SympleCode/Emit/Emitter.h"

int main()
{
	//ShowWindow(GetConsoleWindow(), FALSE);
	SetConsoleTitle(L"Symple Compiler - Treidex");

	FILE* sampleFile;
	errno_t err;
	if (!(err = fopen_s(&sampleFile, "sy/Sample.sy", "rb")) && sampleFile)
	{
		fseek(sampleFile, 0L, SEEK_END);
		unsigned int size = min(ftell(sampleFile), 4096);
		rewind(sampleFile);
		char* source = new char[size + 1];
		fread(source, 1, size, sampleFile);
		source[size] = 0;

		Symple::Parser parser(source);
		Symple::CompilationUnitNode* tree = parser.ParseCompilationUnit();
		Symple::Diagnostics* diagnostics = parser.GetDiagnostics();

		unsigned int parseErrors = diagnostics->GetErrors().size();

		FILE* treeFile;
		errno_t err;
		if (!(err = fopen_s(&treeFile, "sy/Sample.syt", "w")) && treeFile)
		{
			fprintf(treeFile, tree->ToString().c_str());
			fclose(treeFile);
		}
		else
		{
			char errMsg[32];
			if (!strerror_s(errMsg, err))
				std::cerr << "[!]: Error opening tree file: " << errMsg << "!\n";
			else
				std::cerr << "[!]: Unkown Error opening tree file!\n";
		}
		fclose(sampleFile);

		if (parseErrors)
		{
			printf("Compiled with %i errors, %i warnings (total: %i)\n", diagnostics->GetErrors().size(), diagnostics->GetWarnings().size(), diagnostics->GetMessages().size());

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
				Symple::Emitter emitter(diagnostics, "sy/Sample.s");
				emitter.Emit(tree);

				printf("Compiled with %i errors, %i warnings (total: %i)\n", diagnostics->GetErrors().size(), diagnostics->GetWarnings().size(), diagnostics->GetMessages().size());

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
				system("as -c sy/Sample.s -o sy/Sample.o");
				system("ld sy/Sample.o -L msvcrt.lib -o sy/Sample.exe");
				printf("Exit Code: %i\n", system("sy\\Sample"));
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

	std::cin.get();
}