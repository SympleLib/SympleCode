#include <iostream>
#include <string>

#include "SympleCode/Stack.hpp"

int main(int argc, char* args[])
{
	using namespace Symple;

	Stack::SetSize(256 * 1000000); // Set stack size to 256 megabytes

	uint64_t intPtr = Stack::Alloc(4);
	long long num = 1337;
	Stack::Move(4, &num, intPtr);
	std::cout << *(int*)Stack::Get(4, intPtr) << " @" << std::hex << intPtr << std::endl;
	Stack::Free(intPtr);

	std::cin.get();
}