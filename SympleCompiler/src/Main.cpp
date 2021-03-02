#include <iostream>
#include "Symple/Code/Util/Console.h"
#include "Symple/Code/Util/File.h"
#include "Symple/Code/Memory.h"
#include "Symple/Code/Parse/Lexer.h"

using namespace Symple::Code;

int main()
{
	GlobalRef<File> src = MakeRef<File>("sy/Main.sy", FilePermissions::Read);
	Console.Color = ConsoleColor::Yellow;
	std::cout << "Source:\n";
	Console.Color = ConsoleColor::Cyan;
	std::cout << src->Source << '\n';
	
	Lexer lexer(src);
	auto tokens = lexer.LexAll();
	Console.Color = ConsoleColor::Yellow;
	std::cout << "Tokens\n";
	Console.Color = ConsoleColor::Green;
	for (auto tok : tokens)
		std::cout << TokenKindNames[(uint32)tok->GetKind()] << " | " << tok->GetText() << '\n';

	std::cout.flush();
	std::cin.get();
}