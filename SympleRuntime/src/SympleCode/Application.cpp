#include <pch.hpp>

#include <iostream>
#include <string>

#include "SympleCode/VariebleKeeper.hpp"
#include "SympleCode/Stack.hpp"
#include "SympleCode/Heap.hpp"

int main(int argc, char* args[])
{
	using namespace Symple;

	Stack::SetSize(256); // Set stack size to 256 bytes
	Heap::SetSize(256); // Set max heap size to 256 bytes

	int64_t num = 1234567890;

	uint64_t stackPtr = Stack::Alloc(8);
	Stack::Move(8, &num, stackPtr);
	std::cout << "Stack Test: " << *(int64_t*)Stack::Get(8, stackPtr) << " @0x" << std::hex << stackPtr << std::dec << std::endl;
	Stack::Free(8);

	uint64_t heapPtr = Heap::Alloc(8);
	Heap::Move(8, &num, heapPtr);
	std::cout << "Heap Test: " << *(int64_t*)Heap::Get(8, heapPtr) << " @0x" << std::hex << heapPtr << std::dec << std::endl;
	Heap::Free(heapPtr);

	VariebleKeeper::BeginScope();
	VariebleKeeper::Scope("myVar", { 8 });
	uint64_t varPtr = VariebleKeeper::Get("myVar");
	Stack::Move(8, &num, varPtr);
	std::cout << "Varieble Test: " << *(int64_t*)Stack::Get(8, varPtr) << " @0x" << std::hex << varPtr << std::dec << std::endl;
	VariebleKeeper::EndScope();

	std::cin.get();
}