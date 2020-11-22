#include <iostream>
#include <windows.h>

#include "SympleCode/Lexer.hpp"
#include "SympleCode/Parser.hpp"
#include "SympleCode/CodeGen/CodeGenerator.hpp"

int main()
{
	Symple::CodeGenerator::GenerateCode("sy/test.sy", "sy/test.syc");

	//std::cin.get();
}