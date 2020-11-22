#include "SympleCode/CodeGen/CodeGenerator.hpp"

#include <cstdio>
#include <cerrno>
#include <sstream>

#include "SympleCode/Util/Util.hpp"

#include "SympleCode/Tree/AST.hpp"
#include "SympleCode/Parser.hpp"

namespace Symple::CodeGenerator
{
	std::stringstream sOut;

	void GenerateCode(const std::string& in, const std::string& out)
	{
		std::string source;
		// Read Input File
		{
			FILE* fs;
			errno_t err;
			if (!(err = fopen_s(&fs, in.c_str(), "rb")) && fs)
			{
				fseek(fs, 0, SEEK_END);
				long size = ftell(fs);
				rewind(fs);
				char* buff = (char*)alloca(size + 1);
				fread_s(buff, size, sizeof(char), size, fs);
				fclose(fs);
				buff[size] = 0;

				source = buff;
			}
			else
			{
				Err("Error opening file '%s': %s\n", in.c_str(), strerror(err));
				return;
			}
		}

		Branch ast = Parser::Parse(source);
		sOut.clear();
		sOut << ast << '\n';
		HandleBranch(ast);

		// Write Output File
		{
			FILE* fs;
			errno_t err;
			if (!(err = fopen_s(&fs, out.c_str(), "w")) && fs)
			{
				fseek(fs, 0, SEEK_END);
				fputs(sOut.str().c_str(), fs);
				fclose(fs);
			}
			else
			{
				Err("Error opening file '%s': %s\n", in.c_str(), strerror(err));
				return;
			}
		}
	}

	void HandleBranch(const Branch& branch)
	{
		if (branch.Label == AST_BIN)
		{
			//if (std::any_cast<Branch>(branch.FindBranch(AST_LVALUE).Data).Label == AST_CONSTANT)
			//{
			//	if (std::any_cast<ASTToken>(branch.FindBranch(AST_OP).Data) == AST_ADD)
			//	{
			//		sOut << "add " << std::any_cast<int>(std::any_cast<Branch>(branch.FindBranch(AST_LVALUE).Data).Data) << ", "
			//			<< std::any_cast<int>(std::any_cast<Branch>(branch.FindBranch(AST_RVALUE).Data).Data);
			//	}
			//	else if (std::any_cast<ASTToken>(branch.FindBranch(AST_OP).Data) == AST_SUB)
			//	{
			//		sOut << "sub " << std::any_cast<int>(std::any_cast<Branch>(branch.FindBranch(AST_LVALUE).Data).Data) << ", "
			//			<< std::any_cast<int>(std::any_cast<Branch>(branch.FindBranch(AST_RVALUE).Data).Data);
			//	}
			//	else if (std::any_cast<ASTToken>(branch.FindBranch(AST_OP).Data) == AST_MULT)
			//	{
			//		sOut << "mul " << std::any_cast<int>(std::any_cast<Branch>(branch.FindBranch(AST_LVALUE).Data).Data) << ", "
			//			<< std::any_cast<int>(std::any_cast<Branch>(branch.FindBranch(AST_RVALUE).Data).Data);
			//	}
			//	else if (std::any_cast<ASTToken>(branch.FindBranch(AST_OP).Data) == AST_DIV)
			//	{
			//		sOut << "div " << std::any_cast<int>(std::any_cast<Branch>(branch.FindBranch(AST_LVALUE).Data).Data) << ", "
			//			<< std::any_cast<int>(std::any_cast<Branch>(branch.FindBranch(AST_RVALUE).Data).Data);
			//	}
			//}
			//else if (std::any_cast<Branch>(branch.FindBranch(AST_LVALUE).Data).Label == AST_BIN)
			//{
			//	HandleBranch(std::any_cast<Branch>(branch.FindBranch(AST_LVALUE).Data));
			//}
		}
		else
		{
			for (const auto& subBranch : branch.SubBranches)
				HandleBranch(subBranch);
			return;
		}

		sOut << std::endl;
	}
}