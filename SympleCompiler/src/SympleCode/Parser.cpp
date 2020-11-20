#include "SympleCode/Parser.hpp"

#include <vector>
#include <sstream>

#include "SympleCode/Util/Util.hpp"

#include "SympleCode/Tree/Tree.hpp"
#include "SympleCode/Tree/AST.hpp"

#include "SympleCode/Lexer.hpp"

namespace Symple::Parser
{
	static std::vector<std::string> sErrorList;
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
		Walk();

		if (sErrorList.size() <= 0)
		{
			Write("../test/test.tree", sTree);
		}
		else
		{
			Err("Parsing Failed with %zu errors!\n", sErrorList.size());
			for (const auto& err : sErrorList)
				Out("ERROR: %s\n", err.c_str());
		}
	}

	Branch Walk()
	{
		sCurrentTree = &(sTree = ParsePrimaryExpr());

		while (Peek(0).Is(Tokens::Plus) || Peek(0).Is(Tokens::Minus) || Peek(0).Is(Tokens::Asterisk) || Peek(0).Is(Tokens::Slash))
		{
			TokenInfo opTok = PNext();
			Branch right = ParsePrimaryExpr();
			sCurrentTree = &sCurrentTree->PushBranch(AST::BinExpr(IntType, opTok, *sCurrentTree, right));
		}

		return *sCurrentTree;
	}

	Branch ParsePrimaryExpr()
	{
		TokenInfo numTok = Match(Tokens::Number);
		return AST::Constant(IntType, ParseInt(numTok));
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

	TokenInfo Next()
	{
		sCurrentTok++;
		return Peek(0);
	}

	TokenInfo PNext()
	{
		sCurrentTok++;
		return Peek(-1);
	}

	TokenInfo Match(Token tok)
	{
		if (Peek(0).Is(tok))
			return PNext();

		std::stringstream ss;
		ss << "Expected type " << Tokens::ToString(tok) << ", got '" << Peek(0).GetLex() << '\'';

		sErrorList.push_back(ss.str());
		return tok;
	}

#if _WIN64
	int64_t ParseInt(std::string_view view)
#else
	int32_t ParseInt(std::string_view view)
#endif
	{
		int val = 0;
		for (size_t i = 0; i < view.size(); i++)
		{
			if (view[i] < '0' || view[i] > '9')
			{
				std::stringstream ss;
				ss << "Expected int, got '" << view << '\'';

				sErrorList.push_back(ss.str());
				return -1;
			}
			val += (view[i] - '0') * powi(10, view.size() - 1 - i);
		}
		return val;
	}
}