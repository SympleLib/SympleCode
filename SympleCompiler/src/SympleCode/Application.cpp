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
		int size = min(ftell(sampleFile), 4096);
		rewind(sampleFile);
		char* source = new char[size];
		fread(source, 1, size, sampleFile);
		source[size - 1] = 0;

		Symple::Parser parser(source);
		Symple::CompilationUnitNode* tree = parser.ParseCompilationUnit();
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

		{
			Symple::Emitter emitter("sy/Sample.s");
			emitter.Emit(tree);
		}

		system("as -c sy/Sample.s -o sy/Sample.o");
		system("ld sy/Sample.o -o sy/Sample.exe");
		printf("Exit Code: %i\n", system("sy\\Sample"));
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