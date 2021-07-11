#include "Symple/Code/Util/Console.h"
#include "Symple/Code/Util/File.h"

using namespace Symple::Code;
int main()
{
	Console.color = ConsoleColor::Green;

	File file("sy/Main.sy", FilePermissions::Read);
	puts(file.GetSource().c_str());
	(void)getchar();
}