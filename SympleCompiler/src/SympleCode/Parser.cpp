#include "SympleCode/Parser.hpp"

#include <cvt/wstring>

#include <vector>
#include <sstream>

#include "SympleCode/Util/Util.hpp"

#include "SympleCode/Tree/Tree.hpp"
#include "SympleCode/Tree/AST.hpp"

#include "SympleCode/Lexer.hpp"

namespace Symple::Parser
{
	static std::vector<TokenInfo> sTokens;
	static Tree sTree;
	static Branch* sCurrentTree;
	static size_t sCurrentTok;

	static bool myLex(const TokenInfo& tokenInfo)
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

		sCurrentTok = 0;
		sCurrentTree = &(sTree = { "Program" });

		

		COut(sTree);
		Write("../test/test.tree", sTree);
	}

	Branch Walk()
	{
		const TokenInfo* tokInfo = &sTokens[sCurrentTok];

		if (tokInfo->Is(Tokens::Number))
		{
			sCurrentTok++;

			return { "Number Constant", ParseInt(*tokInfo) };
		}
		else if (tokInfo->Is(Tokens::Plus))
		{
			sCurrentTok++;

			return { "Addition", tokInfo->GetLex() };
		}

		sCurrentTok++;

		return { "Unknown Symbol", tokInfo->GetLex() };
	}

	TokenInfo Peek(size_t offset)
	{
		size_t index = sCurrentTok + offset;
		if (index >= sTokens.size())
			return sTokens[sTokens.size() - 1];
		if (index < 0)
			throw std::exception("Expected Token!");
		return sTokens[index];
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