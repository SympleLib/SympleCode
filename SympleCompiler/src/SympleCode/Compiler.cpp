#include "SympleCode/Compiler.h"

#include "SympleCode/Parser/Parser.h"
#include "SympleCode/Emit/Emitter.h"

#include <algorithm>
#include <iostream>
#include <sstream>

namespace Symple
{
	bool Compiler::CompileFile(const std::string& pathStr)
	{
		std::string sytStr = pathStr.substr(0, pathStr.find_last_of('.')) + ".syt";
		std::string asmSStr = pathStr.substr(0, pathStr.find_last_of('.')) + ".s";
		std::string objStr = pathStr.substr(0, pathStr.find_last_of('.')) + ".o";

		const char* path = pathStr.c_str();
		const char* syt = sytStr.c_str();
		const char* asmS = asmSStr.c_str();
		const char* obj = objStr.c_str();

		printf("Preprocessing...\n");
		auto preprocessedSource = mPreprocessor.Preprocess(path);

		for (const Token* token : preprocessedSource)
		{
			std::cout << Token::KindString(token->GetKind()) << " | " << token->GetLex() << '\n';
		}

		printf("Parsing...\n");
		//Parser parser(preprocessedSource);
		Parser parser(path);
		CompilationUnitNode* tree = parser.ParseCompilationUnit();
		Diagnostics* diagnostics = parser.GetDiagnostics();

		FILE* treef;
		errno_t err;
		if (!(err = fopen_s(&treef, syt, "w")) && treef)
		{
			fputs(tree->ToString().c_str(), treef);
			fclose(treef);
		}
		else
		{
			char errMsg[32];
			if (!strerror_s(errMsg, err))
				std::cerr << "[!]: Error opening file '" << syt << "': " << errMsg << "!\n";
			else
				std::cerr << "[!]: Unkown Error opening file '" << syt << "'!\n";
		}

		unsigned int parseErrors = diagnostics->GetErrors().size();
		if (parseErrors)
		{
			printf("Compiled %s with %i errors, %i warnings (total: %i)\n", path, diagnostics->GetErrors().size(), diagnostics->GetWarnings().size(), diagnostics->GetMessages().size());

			for (const Message* error : diagnostics->GetErrors())
			{
				std::cout << "[!]<" << error->Token->GetLine() << ':' << error->Token->GetColumn() << ">: " << error->Message << '\n';
			}

			for (const Message* warning : diagnostics->GetWarnings())
			{
				std::cout << "[?]<" << warning->Token->GetLine() << ':' << warning->Token->GetColumn() << ">: " << warning->Message << '\n';
			}
			printf("No Code Generated :(\n");
		}
		else
		{
			{
				printf("Generating...\n");
				Emitter emitter(diagnostics, asmS);
				emitter.Emit(tree);

				printf("Compiled %s with %i errors, %i warnings (total: %i)\n", path, diagnostics->GetErrors().size(), diagnostics->GetWarnings().size(), diagnostics->GetMessages().size());

				for (const Message* error : diagnostics->GetErrors())
				{
					std::cout << "[!]<" << error->Token->GetLine() << ':' << error->Token->GetColumn() << ">: " << error->Message << '\n';
				}

				for (const Message* warning : diagnostics->GetWarnings())
				{
					std::cout << "[?]<" << warning->Token->GetLine() << ':' << warning->Token->GetColumn() << ">: " << warning->Message << '\n';
				}
			}

			if (!diagnostics->GetErrors().size())
			{
				char command[128];
				sprintf_s(command, "clang -c %s -o %s", asmS, obj);
				system(command);

				mObjectFiles.push_back(objStr);

				return true;
			}
		}

		return false;
	}

	void Compiler::Link(const char* output, const std::vector<const char*> libraries)
	{
		mOutput = output;
		
		std::stringstream ss;
		ss << "clang -o " << output << " --optimize=3";
		for (const std::string& objFile : mObjectFiles)
			ss << ' ' << objFile;
		for (const char* library : libraries)
			ss << " -l " << library;

		puts("Linking Program...");
		system(ss.str().c_str());
	}

	int Compiler::Run()
	{
		puts("Running Program...");
		int exitCode = system(mOutput.c_str());
		printf("\nProgram Exited with Code %i\n", exitCode);

		return exitCode;
	}
}