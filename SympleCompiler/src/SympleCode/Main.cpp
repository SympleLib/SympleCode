#include <Windows.h>
#include <direct.h>

#include "SympleCode/Compiler.h"

static void FindFiles(const std::string& dir);

static std::vector<std::string> sPaths;

std::vector<const char*> sLibraries;

int main(unsigned int argc, const char* argv[])
{
	SetConsoleTitleA("SympleCode Compiler - Treidex");

	FindFiles("sy\\");

	Symple::Compiler compiler;
	bool compiledGood = true;
	for (const std::string& path : sPaths)
	{
		printf("Compiling %s...\n", path.c_str());
		compiledGood &= compiler.CompileFile(path);
	}

	if (compiledGood)
	{
		compiler.Link("sy\\Main.exe", sLibraries);
		compiler.Run();
	}
	
	system("pause");
}

static void FindFiles(const std::string& dir)
{
	WIN32_FIND_DATAA fdata;
	HANDLE hfind = FindFirstFileA((dir + "*").c_str(), &fdata);
	if (hfind == INVALID_HANDLE_VALUE)
		abort();

	do
	{
		if (fdata.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		{
			if (fdata.cFileName[0] != '.')
			{
				std::string newDir = dir + fdata.cFileName + "\\";
				//printf("dir: %s\n", newDir.c_str());

				FindFiles(newDir);
			}
		}
		else
		{
			//printf("file: %s\n", fdata.cFileName);

			std::string name = dir + fdata.cFileName;
			if (name.substr(name.find_last_of('.')) == ".symple")
				sPaths.push_back(name);
		}
	} while (FindNextFileA(hfind, &fdata));
}