#include <pch.h>

#include <cstdlib>
#include <cstdio>

#include "SympleCode/Lex/Lexer.h"

int main()
{
	using namespace Symple;

	Lexer lexer("this_file_doesn't_exist");

	system("pause");
}