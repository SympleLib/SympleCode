#include <Windows.h>
#include <direct.h>

#include "SympleCode/Compiler.h"

static void PrintHelp()
{
	puts("SympleCode Flags:");
	puts("\t-help         | Show Flags");
	puts("\t-c            | Compiles File/s Only");
	puts("\t-r            | Run After Compile");
	puts("\t-i <dir>      | Include Directory");
	puts("\t-l <lib>      | Include Library");
	puts("\t-o <filename> | Output File");
}

struct CompileFlags
{
	bool BadFlag = false;

	bool CompileOnly = false;
	bool RunAfterCompile = false;
	const char* OutputFile = "a.exe";

	const char* IncludeDir = "";

	std::vector<const char*> InputFiles = {};
};

static CompileFlags ParseFlags(unsigned int argc, const char* argv[])
{
	CompileFlags flags;

	for (unsigned int i = 1; i < argc; i++)
	{
		const char* arg = argv[i];

		if (arg[0] == '-')
		{
			const char* flag = &arg[1];

			if (!strcmp(flag, "c"))
			{
				flags.CompileOnly = true;
				continue;
			}

			if (!strcmp(flag, "r"))
			{
				flags.RunAfterCompile = true;
				continue;
			}

			if (!strncmp(flag, "o", 1))
			{
				if (flag[1])
					flags.OutputFile = &flag[1];
				else
					flags.OutputFile = argv[++i];
				continue;
			}

			if (!strncmp(flag, "i", 1))
			{
				if (flag[1])
					flags.IncludeDir = &flag[1];
				else
					flags.IncludeDir = argv[++i];
				continue;
			}

			if (!strncmp(flag, "l", 1))
			{
				if (flag[1])
					sLibraries.push_back(&flag[1]);
				else
					sLibraries.push_back(argv[++i]);
				continue;
			}

			if (!strcmp(flag, "help"))
			{
				PrintHelp();
				continue;
			}

			printf("Bad Flag: '%s'\n", arg);
			flags.BadFlag = true;
			PrintHelp();
			continue;
		}
		else
		{
			flags.InputFiles.push_back(arg);
			continue;
		}
	}

	return flags;
}

static std::vector<const char*> FindFiles(const std::string& dir);

std::vector<const char*> sLibraries;

int main(unsigned int argc, const char* argv[])
{
	CompileFlags flags = ParseFlags(argc, argv);
	if (flags.BadFlag)
		return 1;
	Symple::Compiler compiler(flags.IncludeDir);
	if (flags.InputFiles.empty())
	{
		puts("No Input Files!");
		return 1;
	}

	if (flags.InputFiles.size() == 1)
	{
		if (flags.InputFiles[0] == 0)
			flags.InputFiles = FindFiles("");
		else if (flags.InputFiles[0][strlen(flags.InputFiles[0]) - 1] == '\\' || flags.InputFiles[0][strlen(flags.InputFiles[0]) - 1] == '/')
			FindFiles(flags.InputFiles[0]);
	}

	bool compiledGood = !flags.InputFiles.empty();
	for (const char* path : flags.InputFiles)
	{
		printf("Compiling %s...\n", path);
		compiledGood &= compiler.CompileFile(path);
	}

	if (compiledGood && !flags.CompileOnly)
	{
		compiler.Link(flags.OutputFile, sLibraries);
		if (flags.RunAfterCompile)
			compiler.Run();
	}
}

static std::vector<const char*> FindFiles(const std::string& dir)
{
	std::vector<const char*> paths;

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

				FindFiles(newDir);
			}
		}
		else
		{
			std::string name = dir + fdata.cFileName;
			if (name.substr(name.find_last_of('.')) == ".symple" || name.substr(name.find_last_of('.')) == ".symplecode" || name.substr(name.find_last_of('.')) == ".syc")
			{
				char* cname = new char[1024];
				strcpy_s(cname, 1024, name.c_str());
				paths.push_back(cname);
			}
		}
	} while (FindNextFileA(hfind, &fdata));

	return paths;
}