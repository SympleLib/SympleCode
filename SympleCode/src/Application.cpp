#include <iostream>

#include "scompiler.h"

int main()
{
	using namespace symple;

	serializer compiled = CompileLine("var 5");
	std::cout << compiled << '\n';

	char call = 0;


	system("pause");
}