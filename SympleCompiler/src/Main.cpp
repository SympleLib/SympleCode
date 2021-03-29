#include <iostream>
#include "Symple/Code/Util/Console.h"
#include "Symple/Code/Util/File.h"
#include "Symple/Code/Parse/Lexer.h"
#include "Symple/Code/Parse/Parser.h"
#include "Symple/Code/Visit/Visitor.h"
#include "Symple/Code/Emit/Emitter.h"

using namespace Symple::Code;

int main()
{
	GlobalRef<File> src = MakeRef<File>("sy/Main.sy", FilePermissions::Read);
	Console.Color = ConsoleColor::Yellow;
	std::cout << "Source:\n";
	//Console.Color = ConsoleColor::Cyan;
	//std::cout << src->Source << '\n';
	
	Lexer lexer(src);
	auto tokens = lexer.LexAll();

	uint32 pln = 1, pcol = 1;
	for (auto tok : tokens)
	{
		for (uint32 ln = tok->Line; ln > pln; ln--)
		{
			std::cout << '\n';
			pcol = 0;
		}

		for (uint32 col = tok->Column - 1; col > pcol; col--)
			std::cout << ' ';

		uint32 kind = (uint32)tok->Kind;

		if (kind >= (uint32)TokenKind::Keyword)
			Console.Color = ConsoleColor::Magenta;
		else if (kind >= (uint32)TokenKind::Punctuator)
			Console.Color = ConsoleColor::Cyan;
		else if (kind == (uint32)TokenKind::Number)
			Console.Color = ConsoleColor::DarkYellow;
		else if (kind == (uint32)TokenKind::Identifier)
			Console.Color = ConsoleColor::White;
		else
			Console.Color = ConsoleColor::Red;
		std::cout << tok->Text;

		pln = tok->Line;
		pcol = tok->Column + tok->Text.length() - 1;
	}
	std::cout << '\n';

	//Console.Color = ConsoleColor::Yellow;
	//std::cout << "Tokens:\n";
	//Console.Color = ConsoleColor::Green;
	//for (auto tok : tokens)
	//	std::cout << tok->Kind << " | " << tok->Text << " <" << tok->Line << ':' << tok->Column << ">\n";

	Parser parser(tokens);
	auto unit = parser.Parse();

	SymbolVisitor symbolVisit(unit);
	Console.Color = ConsoleColor::Red;
	symbolVisit.Visit();

	TypeVisitor typeVisit(unit);
	Console.Color = ConsoleColor::Red;
	typeVisit.Visit();

	Console.Color = ConsoleColor::Yellow;
	std::cout << "Ast:\n";
	Console.Color = ConsoleColor::Cyan;
	unit->Print(std::cout);
	std::cout << '\n';

	Emitter emmiter(unit);
	emmiter.Emit();

	system("gcc -m32 bin/Out.S -o bin/Out.exe --debug");
	Console.Color = ConsoleColor::Yellow;
	puts("Starting program...");
	Console.Color = ConsoleColor::Reset;
	union
	{
		int ec;
		float fec;
	};
	ec = system("bin\\Out.exe");
	Console.Color = ConsoleColor::Yellow;
	printf("\nProgram exited with code %i (0x%x) [%g]", ec, ec, fec);

	std::cout.flush();
	std::cin.get();
}