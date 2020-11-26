#include <iostream>

#include "SympleCode/CodeGen/CodeGenerator.hpp"

int main()
{
	using namespace Symple;
	CodeGenerator::GenerateCode("sy/test.sy", "test.exe");

	//std::cin.get();
}