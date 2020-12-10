#include <windows.h>
#include <iostream>
#include <cstring>

#include "SympleCode/Parser/Parser.h"

int main()
{
	SetConsoleTitle(L"Symple Compiler - Treidex");

	FILE* sampleFile;
	errno_t err;
	if (!(err = fopen_s(&sampleFile, "sy/Sample.sy", "r")) && sampleFile)
	{
		fseek(sampleFile, 0, SEEK_END);
		size_t size = (size_t)ftell(sampleFile) + 1;
		rewind(sampleFile);
		char* source = new char[size];
		fread(source, 1, size - 1, sampleFile);
		source[size - 1] = 0;

		Symple::Parser parser(source);
		Symple::CompilationUnitNode* tree = parser.ParseCompilationUnit();
		std::cout << tree->ToString();
	}
	else
	{
		char errMsg[32];
		if (strerror_s(errMsg, err))
			std::cerr << "[!]: Error opening sample file: " << errMsg << "!\n";
		else
			std::cerr << "[!]: Unkown Error opening sample file!\n";
	}
	std::cin.get();
}