#include <iostream>
#include <cstdlib>

#include <spdlog/spdlog.h>

#include "SympleCode/Compiler.h"
#include "SympleCode/Util/ConsoleColor.h"

using namespace Symple::Util;

using std::shared_ptr;
using std::make_shared;

using std::unique_ptr;
using std::make_unique;

using spdlog::level::level_enum;

int main()
{
	spdlog::set_pattern("[Symple]%^<%l>%$: %v");
	spdlog::set_level(level_enum::trace);
	SetConsoleColor(Yellow);

	unique_ptr<Symple::Compiler> compiler = make_unique<Symple::Compiler>((char*)"sy/Main.sy");
	compiler->Lex();
	compiler->Parse();
	compiler->Bind();
	compiler->Emit();
	compiler->Exec();

	return !getc(stdin);
}