#include <iostream>

#include "SympleCode/CodeGen/Generator.hpp"

int main()
{
	using namespace Symple;
	ASM::Open("sy/test.syc");
	ASM::Alloc(Type::Int);
	ASM::Alloc(Type::Long);

	int mydouble = 69;

	ASM::Set(&mydouble, ASM::Alloc(Type::Double), 8);
	ASM::DeAlloc(Type::Int);
	ASM::DeAlloc(Type::Long);
	ASM::DeAlloc(Type::Double);
	ASM::Close();
}