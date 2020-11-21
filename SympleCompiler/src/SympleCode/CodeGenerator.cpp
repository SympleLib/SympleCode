#include "SympleCode/CodeGenerator.hpp"

#include <cstdio>
#include <sstream>

#include "SympleCode/Util/Util.hpp"

#include "SympleCode/Tree/AST.hpp"
#include "SympleCode/Parser.hpp"

namespace Symple::CodeGenerator
{
	void GenerateCode(const std::string& in, const std::string& out)
	{
		std::string source;
		// Read Input File
		{
			FILE* fs;
			errno_t err;
			if (!(err = fopen_s(&fs, in.c_str(), "rb") || fs))
			{
				fseek(fs, 0, SEEK_END);
				long size = ftell(fs);
				rewind(fs);
				char* buff = (char*)alloca(size + 1);
				fread(buff, sizeof(char), size, fs);
				fclose(fs);
				buff[size] = 0;

				source = buff;
			}
			else
			{
				Err("Error opening file '%s': %i", in.c_str(), err);
				return;
			}
		}

		Branch ast = Parser::Parse(source);
		std::stringstream ss;
		HandleBranch(ast, &ss);
		COut(ss.str());
	}

	void HandleBranch(const Branch& branch, std::stringstream* ss)
	{
		if (branch.Label == AST_ADD)
		{
			*ss << "add " << std::any_cast<int>(branch.FindBranch(AST_LVALUE).Data) << ", " << std::any_cast<int>(branch.FindBranch(AST_RVALUE).Data);
		}
		else if (branch.Label == AST_SUB)
		{
			*ss << "sub " << std::any_cast<int>(branch.FindBranch(AST_LVALUE).Data) << ", " << std::any_cast<int>(branch.FindBranch(AST_RVALUE).Data);
		}
		else if (branch.Label == AST_MULT)
		{
			*ss << "mul " << std::any_cast<int>(branch.FindBranch(AST_LVALUE).Data) << ", " << std::any_cast<int>(branch.FindBranch(AST_RVALUE).Data);
		}
		else if (branch.Label == AST_DIV)
		{
			*ss << "div " << std::any_cast<int>(branch.FindBranch(AST_LVALUE).Data) << ", " << std::any_cast<int>(branch.FindBranch(AST_RVALUE).Data);
		}

		*ss << std::endl;
	}
}