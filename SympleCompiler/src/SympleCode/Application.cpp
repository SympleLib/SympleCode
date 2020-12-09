#include <windows.h>
#include <iostream>

#include "SympleCode/Parser/Parser.h"

int main()
{
	SetConsoleTitle(L"Symple Compiler - Treidex");

	Symple::Parser parser("6 + 9");
	Symple::CompilationUnitNode* tree = parser.ParseCompilationUnit();
	const std::list<Symple::MemberNode*>& members = tree->GetMembers();
	for (const Symple::MemberNode* member : members)
	{

	}

	std::cin.get();
}