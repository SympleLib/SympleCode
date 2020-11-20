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
	static std::vector<Type> sTypes;
	static Tree sTree;
	static size_t sCurrentTok;

	static bool myLex(const TokenInfo& tokenInfo)
	{
		sTokens.push_back(tokenInfo);
		if (tokenInfo.IsEither(Tokens::End, Tokens::Unexpected))
		{
			if (tokenInfo.Is(Tokens::Unexpected))
				CErr("Unexpected Token: '" << tokenInfo.GetLex() << "'\n");

			return false;
		}
		//COut(Tokens::ToString(tokenInfo.GetToken()) << " '" << tokenInfo.GetLex() << "'\n");
		return true;
	}

	void Parse(std::string& source)
	{
		sTokens.clear();
		Lexer::Lex(source.c_str(), myLex);
		
		sCurrentTok = 0;
		sTypes = {
			VoidType,
			IntType
		};
		sErrorList.clear();

		ParseMembers();

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

	Branch ParseMembers()
	{
		sTree = {};

		while (Peek().IsNot(Tokens::End))
		{
			TokenInfo start = Peek();

			sTree.PushBranch(ParseMember());

			if (Peek() == start)
				Next();
		}

		return sTree;
	}

	Branch ParseMember()
	{
		if (Peek().IsKeyWord(KeyWords::Function))
			return ParseFuncDecl();
		return ParseStatement();
	}

	Branch ParseFuncDecl()
	{
		Next();
		Type type = ParseType();
		std::string name(Match(Tokens::Identifier));
		Branch params = ParseParams();
		COut(params);
		Branch body = ParseBlock();
		
		return AST::Func(type, name, params, body);
	}

	Branch ParseParams()
	{
		Match(Tokens::LeftParen);

		Branch list;
		bool parseNext = true;
		while (parseNext && Peek().IsNot(Tokens::RightParen) && Peek().IsNot(Tokens::End))
		{
			Branch param = ParseParam();
			list.PushBranch(param);

			if (parseNext = Peek().Is(Tokens::Comma))
				Next();
		}
		Next();

		return list;
	}

	Branch ParseParam()
	{
		Type type = ParseType();
		std::string name(Match(Tokens::Identifier).GetLex());
		return AST::Param(type, name);
	}
	
	Branch ParseStatement()
	{
		switch (Peek().GetToken())
		{
		default:
			return ParseExpr();
		}
	}

	Branch ParseBlock()
	{
		Branch block;
		Match(Tokens::LeftCurly);
		while (Peek().IsNot(Tokens::End) && Peek().IsNot(Tokens::RightCurly))
		{
			TokenInfo start = Peek();

			Branch statement = ParseStatement();
			block.PushBranch(statement);

			if (Peek() == start)
				Next();
		}
		Match(Tokens::RightCurly);

		return block;
	}

	Branch ParseExpr()
	{
		return ParseBinExpr();
	}

	Branch ParseSetExpr()
	{
		if (Peek(0).Is(Tokens::Identifier) && Peek(1).Is(Tokens::EqualsEqual))
		{
			TokenInfo idTok = PNext();
			Next();
			Branch right = ParseSetExpr();
			return AST::Set(IntType, idTok, right);
		}
		return ParseBinExpr();
	}

	Branch ParseBinExpr(int8_t parentOOO)
	{
		Branch left;
		int8_t uooo = GetUnOpOOO(Peek(0).GetToken());
		if (uooo >= 0 && uooo >= parentOOO)
		{
			TokenInfo opTok = PNext();
			Branch value = ParseBinExpr(uooo);
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
			Branch right = ParseBinExpr(ooo);
			left = AST::BinExpr(IntType, opTok, left, right);
		}

		return left;
	}

	Branch ParsePrimaryExpr()
	{
		switch (Peek(0).GetToken())
		{
		case Tokens::LeftParen:
		{
			Next();
			Branch expr = ParseExpr();
			Match(Tokens::RightParen);
			return expr;
		}
		case Tokens::Comment:
		{
			return AST::Comment(Peek(0));
		}
		}
		TokenInfo numTok = Match(Tokens::Number);
		return AST::Constant(IntType, ParseInt(numTok));
	}

	Type ParseType()
	{
		TokenInfo tok = PNext();
		for (const auto& ty : sTypes)
			if (ty.Name == tok.GetLex())
				return ty;
		return VoidType;
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

	int8_t GetLogOpOOO(Token tok)
	{
		switch (tok)
		{
			using namespace Tokens;
		case EqualsEqual:
		case NotEqual:
			return 0;
		}
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