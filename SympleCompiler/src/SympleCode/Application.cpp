#include <windows.h>
#include <iostream>

#include "SympleCode/Parser/Parser.h"

int main()
{
	SetConsoleTitle(L"Symple Compiler - Treidex");

	Symple::Parser parser("9.6 = 1 + 5 / 1 + 2 * 3");
	Symple::CompilationUnitNode* tree = parser.ParseCompilationUnit();
	std::cout << tree->ToString();

	std::cin.get();
}