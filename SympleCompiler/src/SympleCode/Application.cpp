#include <iostream>
#include <windows.h>

#include "SympleCode/Lexer.hpp"
#include "SympleCode/Parser.hpp"
#include "SympleCode/CodeGenerator.hpp"

int main()
{
	Symple::CodeGenerator::GenerateCode("D:/Projects/C++/SympleCode/SympleCompiler/sy/test.sy", "D:/Projects/C++/SympleCode/SympleCompiler/sy/test.syc");

	std::cin.get();
}