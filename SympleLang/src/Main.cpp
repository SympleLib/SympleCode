﻿#include <iostream>
#include <vector>
#include <cstdlib>

#include <spdlog/spdlog.h>
#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>

#include "SympleCode/Compiler.h"
#include "SympleCode/Util/FileUtil.h"
#include "SympleCode/Util/ConsoleColor.h"

using namespace Symple::Util;

using std::shared_ptr;
using std::make_shared;

using std::unique_ptr;
using std::make_unique;

using spdlog::level::level_enum;

int main()
{
	spdlog::sinks_init_list sinks = { /* make_shared<spdlog::sinks::stdout_color_sink_mt>(), */ make_shared<spdlog::sinks::basic_file_sink_mt>("stdout.log", true) };
	spdlog::set_default_logger(make_shared<spdlog::logger>("Symple Logger", sinks));
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