#include "SympleCode/Parser.hpp"

#include <vector>
#include <sstream>

#include "SympleCode/Util/Util.hpp"

#include "SympleCode/Tree/Tree.hpp"

#include "SympleCode/Lexer.hpp"

namespace Symple::Parser
{
	static std::vector<Lexer::TokenInfo> sTokens;
	static Tree sTree;
	static Branch* sCurrentTree;

	static bool myLex(const Lexer::TokenInfo& tokenInfo)
	{
		if (tokenInfo.IsEither(Tokens::End, Tokens::Unexpected))
		{
			if (tokenInfo.Is(Tokens::Unexpected))
				CErr("Unexpected Token: '" << tokenInfo.GetLex() << "'\n");

			return false;
		}

		sTokens.push_back(tokenInfo);
		COut(Tokens::ToString(tokenInfo.GetToken()) << " '" << tokenInfo.GetLex() << "'\n");
		return true;
	}

	void Parse(const char* source)
	{
		Lexer::Lex(source, myLex);
		sCurrentTree = &(sTree = {});

		size_t beg = 0;

	ParseNext:
		if (sTokens.size() - beg > 3)
		{
			if (sTokens[beg].Is(Tokens::Identifier))
			{
				if (sTokens[beg].GetLex() == "int")
				{
					Branch& varBranch = sCurrentTree->PushBranch("Var");
					varBranch.PushBranch("Name", sTokens[beg + 1].GetLex());
					varBranch.PushBranch("Type", "int");

					if (sTokens.size() + beg <= 2 || !sTokens[beg + 2].Is(Tokens::Semicolon))
						CErr("Syntax Error: 'Semicolon Expected'\n");
					beg += 3;
					goto ParseNext;
				}
			}

			if (sTokens[beg + 1].Is(Tokens::Equal))
			{
				Branch& setBranch = sCurrentTree->PushBranch("Op. =");
				Branch& lvalBranch = setBranch.PushBranch("L Value");
				lvalBranch.PushBranch("Name", sTokens[beg].GetLex());
				lvalBranch.PushBranch("Type", "int");
				Branch& rvalBranch = setBranch.PushBranch("R Value");
				rvalBranch.PushBranch("Value", ParseInt(sTokens[beg + 2].GetLex()));

				if (sTokens.size() + beg <= 3 || !sTokens[beg + 3].Is(Tokens::Semicolon))
					CErr("Syntax Error: 'Semicolon Expected'\n");
				beg += 4;
				goto ParseNext;
			}
		}

		COut(sTree);
		Write("../test/test.tree", sTree);
	}

#ifdef WIN32
	int32_t ParseInt(std::string_view view)
#else
	int64_t ParseInt(std::string_view view)
#endif
	{
		int val = 0;
		for (size_t i = 0; i < view.size(); i++)
		{
			val += (view[i] - '0') * powi(10, view.size() - 1 - i);
		}
		return val;
	}
}