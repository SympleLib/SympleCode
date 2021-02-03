#include <iostream>
#include <cstdlib>

#include <spdlog/spdlog.h>

#include "SympleCode/Syntax/Lexer.h"
#include "SympleCode/Syntax/Parser.h"
#include "SympleCode/Binding/Binder.h"

using namespace Symple;
using namespace Symple::Binding;
using namespace Symple::Syntax;
using namespace Symple::Symbol;

using std::shared_ptr;
using std::make_shared;

using std::unique_ptr;
using std::make_unique;

using spdlog::level::level_enum;

#ifdef _WIN32
enum ConsoleColor
{
	Black,
	DarkBlue,
	DarkGreen,
	DarkCyan,
	DarkRed,
	DarkMagenta,
	DarkYellow,
	Grey,
	DarkGrey,
	Blue,
	Green,
	Cyan,
	Red,
	Magenta,
	Yellow,
	White,

	Reset = White,
};
#endif

bool PrintDiagnosticBag(shared_ptr<DiagnosticBag> diagnostics, char step[] = "Null Step")
{
#ifdef _WIN32
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), Yellow);
#endif

	unsigned errCount = 0, warningCount = 0, messageCount = 0;

	for (shared_ptr<Diagnostic> diagnostic : diagnostics->GetDiagnostics())
		switch (diagnostic->GetLevel())
		{
		case Diagnostic::Message:
			spdlog::info("{}", diagnostic->GetMessage());
			messageCount++;
			break;
		case Diagnostic::Warning:
			spdlog::warn("{}", diagnostic->GetMessage());
			warningCount++;
			break;
		case Diagnostic::Error:
			spdlog::error("{}", diagnostic->GetMessage());
			errCount++;
			break;
		}

	if (errCount)
	{
		spdlog::info("{} failed with {} errors, {} warnings, {} messages", step, errCount, warningCount, messageCount);

		return true;
	}
	else
	{
		spdlog::info("{} completed with {} errors, {} warnings, {} messages", step, errCount, warningCount, messageCount);

		return false;
	}
}
shared_ptr<Lexer> lexer;
std::vector<shared_ptr<Token>> tokens;

shared_ptr<FunctionDeclarationSyntax> node;

shared_ptr<Symple::Symbol::Symbol> bound;

void Lex()
{
	lexer = make_shared<Lexer>((char*)"sy/Main.sy");
	lexer->SetRef(lexer);
	tokens.clear();

	spdlog::info("Lex Tokens:");
	do
	{
		tokens.push_back(lexer->Lex());
		tokens.back()->Print(std::cout, "", tokens.back()->Is(Token::EndOfFile));
		putchar('\n');
	}
	while (!tokens.back()->Is(Token::EndOfFile));

	if (PrintDiagnosticBag(lexer->GetDiagnosticBag(), "Lexing"))
		tokens.clear();
}

void Parse()
{
	shared_ptr<Parser> parser = make_shared<Parser>(lexer, tokens);
	node = parser->ParseFunctionDeclaration();
	putchar('\n');
	if (PrintDiagnosticBag(parser->GetDiagnosticBag(), "Parsing"))
		return;

	spdlog::info("Parse Tree:");
	node->Print();
	putchar('\n');
}

void Bind()
{
	if (!node)
		return;

	shared_ptr<Binder> binder = make_shared<Binder>();
	bound = binder->BindFunction(node);
	putchar('\n');
	if (PrintDiagnosticBag(binder->GetDiagnosticBag(), "Binding"))
		return;

	spdlog::info("Bound Tree:");
	bound->Print();
	putchar('\n');

#ifdef _WIN32
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), Reset);
#endif
}

int main()
{
	spdlog::set_pattern("[Symple]%^<%l>%$: %v");
	spdlog::set_level(level_enum::trace);
#ifdef _WIN32
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), Yellow);
#endif

	Lex();
	Parse();
	Bind();

	return !getc(stdin);
}