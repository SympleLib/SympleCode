#include <Windows.h>

#include "SympleCode/Compiler.h"

int main(unsigned int argc, const char* argv[])
{
	SetConsoleTitleA("SympleCode Compiler - Treidex");

	Symple::Compiler compiler;
	if (compiler.CompileFile("Main.symple"))
	{
		compiler.Link("Main.exe");
		compiler.Run();
	}

	system("pause");
}