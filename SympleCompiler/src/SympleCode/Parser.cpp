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
			if (tokenInfo.Is(Tokens::Unexpected));
				CErr("Unexpected Token: '" << tokenInfo.GetLex() << "'\n");

			return false;
		}

		sTokens.push_back(tokenInfo);
		COut(Tokens::ToString(tokenInfo.GetToken()) << " '" << "'\n");
		return true;
	}

	void Parse(const char* beg)
	{
		Lexer::Lex(beg, myLex);
		sCurrentTree = &(sTree = {});

		if (sTokens[0].Is(Tokens::Identifier))
		{
			//if (sTokens[0].GetLex() == "int")
			{
			}
		}

		//COut(sTree);
		Write("../tree/test.tree", sTree);
	}

#ifdef WIN32
	int32_t ParseInt(const std::string_view& view)
#else
	int64_t ParseInt(const std::string& view)
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