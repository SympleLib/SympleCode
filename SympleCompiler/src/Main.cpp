#include <iostream>
#include "Symple/Code/Util/Console.h"
#include "Symple/Code/Util/File.h"
#include "Symple/Code/Memory.h"

using namespace Symple::Code;

int main()
{
	GlobalRef<File> src = MakeRef<File>("sy/Main.sy", FilePermissions::Read);
	Console.Color = ConsoleColor::Yellow;
	std::cout << "Source:\n";
	Console.Color = ConsoleColor::Cyan;
	std::cout << src->Source << '\n';
	std::cin.get();
}