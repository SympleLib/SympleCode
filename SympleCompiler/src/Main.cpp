#include <iostream>
#include "Symple/Syntax/Token.h"

using namespace Symple;

int main()
{
	Scope<File> file = MakeScope<File>("sy/Main.sy", FilePermissions::Read);
	std::cout << file->Source << std::endl;

	return !getchar();
}