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

	void Parse(std::string& source)
	{
		sTokens.clear();
		Lexer::Lex(source.c_str(), myLex);
		
		sCurrentTree = &(sTree = { "Program" });
		sCurrentTok = 0;
		sErrorList.clear();

		sCurrentTree->PushBranch(ParseExpr());

		if (sErrorList.size() <= 0)
		{
			//COut(sTree);
			Write("../test/test.tree", sTree);
		}
		else
		{
			Err("Parsing Failed with %zu errors!\n", sErrorList.size());
			for (const auto& err : sErrorList)
				Err("ERROR: %s!\n", err.c_str());
			std::cin.get();
		}
	}

	Branch ParsePrimaryExpr()
	{
		if (Peek(0).Is(Tokens::LeftParen))
		{
			TokenInfo left = PNext();
			Branch expr = ParseExpr();
			TokenInfo right = Match(Tokens::RightParen);
			return AST::ParenExpr(IntType, left, expr, right);
		}
		else if (Peek(0).Is(Tokens::Identifier))
		{
			TokenInfo id = Peek(0);
			return AST::Id(id);
		}
		TokenInfo numTok = Match(Tokens::Number);
		return AST::Constant(IntType, ParseInt(numTok));
	}

	Branch ParseExpr(int8_t parentOOO)
	{
		Branch left;
		int8_t uooo = GetUnOpOOO(Peek(0).GetToken());
		if (uooo >= 0 && uooo >= parentOOO)
		{
			TokenInfo opTok = PNext();
			Branch value = ParseExpr(uooo);
			left = AST::UnExpr(IntType, opTok, value);
		}
		else
			left = ParsePrimaryExpr();

		while (true)
		{
			int8_t ooo = GetBinOpOOO(Peek(0).GetToken());
			if (ooo < 0 || ooo <= parentOOO)
				break;
			TokenInfo opTok = PNext();
			Branch right = ParseExpr(ooo);
			left = AST::BinExpr(IntType, opTok, left, right);
		}

		return left;
	}

	int8_t GetBinOpOOO(Token tok)
	{
		switch (tok)
		{
			using namespace Tokens;
		case Plus:
		case Minus:
			return 0;
		case Asterisk:
		case Slash:
			return 1;
		}

		return -1;
	}

	int8_t GetUnOpOOO(Token tok)
	{
		switch (tok)
		{
			using namespace Tokens;
		case Plus:
		case Minus:
			return 2;
		}

		return -1;
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
		ss << "Unexpected token <" << Tokens::ToString(Peek(0).GetToken()) << ">, expected <" << Tokens::ToString(tok) << '>';

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