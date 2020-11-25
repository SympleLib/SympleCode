#include "SympleCode/CodeGen/CodeGenerator.hpp"

#include <cstdio>
#include <cerrno>
#include <sstream>

#include "SympleCode/Util/Util.hpp"

#include "SympleCode/Tree/AST.hpp"
#include "SympleCode/CodeGen/Generator.hpp"
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
			if (!(err = fopen_s(&fs, in.c_str(), "rb")) && fs)
			{
				fseek(fs, 0, SEEK_END);
				size_t size = ftell(fs);
				rewind(fs);
#pragma warning(push)
#pragma warning(disable: 6255)
				char* buff = (char*)alloca(size + 1);
#pragma warning(pop)
				fread_s(buff, size, sizeof(char), size, fs);
				fclose(fs);
				buff[size] = 0;

				source = buff;
			}
			else
			{
				char errMsg[256];
				if (!strerror_s(errMsg, err))
					Err("Error opening file '%s': %s\n", in.c_str(), errMsg);
				return;
			}
		}

		Branch ast = Parser::Parse(source);
		ASM::Open(out);
		HandleBranch(ast);
		ASM::Close();
	}

	void HandleBranch(const Branch& branch)
	{
		if (branch.Label == AST_BIN)
			ASM::BinExpr(branch);
		else if (branch.Label == AST_VAR_DECL)
			ASM::VarDecl(branch);
		else if (branch.Label == AST_FUNC_DECL)
		{
			ASM::StartFunc(branch.FindBranch(AST_NAME).Cast<std::string>().c_str());
			HandleBranch(branch.FindBranch(AST_BODY).Cast<Branch>());
			ASM::EndFunc(branch.FindBranch(AST_TYPE).Cast<Type>());
		}
		else if (branch.Label == AST_RETURN)
			ASM::Return(branch);
		else
			for (const auto& subBranch : branch.SubBranches)
				HandleBranch(subBranch);
	}
}