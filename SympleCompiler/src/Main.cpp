#include <iostream>
#include "Symple/Common/Console.h"
#include "Symple/Common/Memory.h"
#include "Symple/Common/File.h"

using namespace Symple;

int main()
{
	GlobalRef<File> src = MakeGlobalRef<File>("sy/Main.sy", FilePermissions::Read);
	Console.Color = ConsoleColor::Yellow;
	std::cout << "Source:\n";
	Console.Color = ConsoleColor::Cyan;
	std::cout << src->Source << '\n';
	std::cin.get();
}