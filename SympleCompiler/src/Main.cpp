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

	const char *p = src->Source.c_str();
	for (auto tok : tokens)
	{
		Console.Color = ConsoleColor::Grey;
		while (p != tok->Text.data())
			std::cout << *p++;


		if ((uint32)tok->Kind >= (uint32)TokenKind::Keyword)
			Console.Color = ConsoleColor::Magenta;
		else if ((uint32)tok->Kind >= (uint32)TokenKind::Punctuator)
			Console.Color = ConsoleColor::Cyan;
		else switch (tok->Kind)
		{
		case TokenKind::Number:
			Console.Color = ConsoleColor::DarkYellow;
			break;
		case TokenKind::Identifier:
			Console.Color = ConsoleColor::White;
			break;
		case TokenKind::Char:
		case TokenKind::String:
			Console.Color = ConsoleColor::Green;
			break;

		default:
			Console.Color = ConsoleColor::Red;
			break;
		}
		
		std::cout << tok->Text;
		p = tok->Text.data() + tok->Text.length();
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

	//Console.Color = ConsoleColor::Yellow;
	//std::cout << "Ast:\n";
	//Console.Color = ConsoleColor::Cyan;
	//unit->Print(std::cout);
	//std::cout << '\n';

	Emitter emmiter(unit);
	emmiter.Emit();

	system("clang -m32 bin/Out.S -o bin/Out.exe --debug -l User32");
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
	printf("\nProgram exited with code %i (0x%x) [%g]\n", ec, ec, fec);
	Console.Color = ConsoleColor::White;

	std::cout.flush();
	std::cin.get();
}