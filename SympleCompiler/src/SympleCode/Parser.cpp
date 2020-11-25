#include "SympleCode/Parser.hpp"

#include <sstream>

#include "SympleCode/Util/Util.hpp"

#include "SympleCode/Tree/Tree.hpp"
#include "SympleCode/Tree/AST.hpp"

#include "SympleCode/Type.hpp"

#include "SympleCode/Lexer.hpp"

namespace Symple::Parser
{
	static std::vector<std::string> sErrorList;
	static std::vector<TokenInfo> sTokens;
	std::vector<Varieble> sVars;
	std::vector<Function> sFuncs;
	std::map<std::string, Type> sTypes;
	std::map<std::string, Param> sParams;
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

	Branch Parse(std::string& source)
	{
		sTokens.clear();
		Lexer::Lex(source.c_str(), myLex);
		
		sCurrentTok = 0;
		sVars.clear();
		sFuncs.clear();
		sTypes = {
			{ "void", Type::Void },

			{ "byte", Type::Byte },
			{ "short", Type::Short },
			{ "int", Type::Int },
			{ "long", Type::Long },

			{ "sbyte", Type::SByte },
			{ "sshort", Type::SShort },
			{ "sint", Type::SInt },
			{ "slong", Type::SLong },

			{ "ubyte", Type::UByte },
			{ "ushort", Type::UShort },
			{ "uint", Type::UInt },
			{ "ulong", Type::ULong },
		};
		sErrorList.clear();

		ParseMembers();

		if (sErrorList.size() <= 0)
		{
			Write("../test/test.tree", sTree);
		}
		else
		{
			Err("Parsing Failed with %zu errors!\n", sErrorList.size());
			for (const auto& err : sErrorList)
				Err("ERROR: %s!\n", err.c_str());
			std::cin.get();
		}

		return sTree;
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
		Branch body = ParseBlock();

		std::vector<Param> pparams;
		for (const auto& pair : sParams)
			pparams.push_back(pair.second);

		sFuncs.push_back({ name, type, pparams });
		return AST::FuncDecl(type, name, params, body);
	}

	Branch ParseParams()
	{
		Match(Tokens::LeftParen);

		Branch list;
		bool parseNext = true;
		while (parseNext && Peek().IsNot(Tokens::RightParen) && Peek().IsNot(Tokens::End))
		{
			Branch param = ParseParam();
			if (param != Branch())
				list.PushBranch(param);

			if (parseNext = Peek().Is(Tokens::Comma))
				Next();
		}
		Next();
		sParams.clear();

		return list;
	}

	Branch ParseParam()
	{
		Type type = ParseType();
		std::string name(Match(Tokens::Identifier).GetLex());

		if (sParams.find(name) != sParams.end())
		{
			Err("Parameter '%s' already exists!\n", name.c_str());
			abort();
		}
		sParams[name] = { type };

		return AST::Param(type);
	}

	Branch ParseParams(const Function& func)
	{
		Match(Tokens::LeftParen);

		Branch list;
		bool parseNext = true;
		size_t nparam = 0;
		while (parseNext && Peek().IsNot(Tokens::RightParen) && Peek().IsNot(Tokens::End))
		{
			Branch param = ParseParam(func, nparam);
			if (param != Branch())
				list.PushBranch(param);

			nparam++;
			if (parseNext = Peek().Is(Tokens::Comma))
				Next();
		}
		Next();

		return list;
	}

	Branch ParseParam(const Function& func, size_t nparam)
	{
		Type type = func.Params[nparam];
		return AST::Param(type);
	}
	
	Branch ParseStatement()
	{
		switch (Peek().AsKeyWord())
		{
		case KeyWords::Varieble:
			return ParseVarDecl();
		case KeyWords::Return:
			return ParseReturn();
		default:
			return {}; // Tiny optimization for file size, but I'm not going to do it when I'm testing
			//return ParseExpr();
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
			if (statement != Branch())
				block.PushBranch(statement);

			if (Peek() == start)
				Next();
		}
		Next();

		return block;
	}

	Branch ParseReturn()
	{
		Next();
		Type ty = ParseType();
		Branch ret = ParseExpr();
		return AST::Return(ty, ret);
	}

	Branch ParseExpr()
	{
		return ParseBinExpr();
	}

	Branch ParseVarDecl()
	{
		Next();
		Type type = ParseType();
		std::string name(Match(Tokens::Identifier).GetLex());
		Match(Tokens::Equal);
		Branch init = ParseExpr();
		sVars.push_back({ name, type });
		return AST::VarDecl(type, name, init);
	}

	Branch ParseBinExpr(int8_t parentOOO)
	{
		Branch left;
		int8_t uooo = GetUnOpOOO(Peek(0).GetToken());
		if (uooo >= 0 && uooo >= parentOOO)
		{
			TokenInfo opTok = PNext();
			Branch value = ParseBinExpr(uooo);
			left = AST::UnExpr(Type::Int, opTok, value);
		}
		else
		{
			if (Peek().Is(Tokens::Semicolon))
				return {};
			left = ParsePrimaryExpr();
		}

		while (true)
		{
			int8_t ooo = GetBinOpOOO(Peek().GetToken());
			if (ooo < 0 || ooo <= parentOOO)
				break;
			TokenInfo opTok = PNext();
			Branch right = ParseBinExpr(ooo);
			left = AST::BinExpr(Type::Int, opTok, left, right);
		}

		return left;
	}

	Branch ParsePrimaryExpr()
	{
		switch (Peek().GetToken())
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
			Next();
			return ParsePrimaryExpr();
		}
		case Tokens::Identifier:
		{
			if (Peek(1).Is(Tokens::LeftParen))
				return AST::FuncCall(std::string(Peek().GetLex()), ParseParams());
			for (const auto& var : sVars)
				if (var.Name == Peek().GetLex())
					return AST::VarVal(var.Type, var.Name);
			for (const auto& pair : sParams)
				if (pair.first == Peek().GetLex())
					return AST::ParamVal(pair.second);
			Err("Unexpected Identifier: '%s'", std::string(Peek().GetLex()).c_str());
			abort();
		}
		}
		TokenInfo numTok = Match(Tokens::Number);
#pragma warning(push)
#pragma warning(disable: 4244)
		return AST::Constant(Type::Int, ParseInt(numTok));
#pragma warning(pop)
	}

	Type ParseType()
	{
		TokenInfo tok = PNext();
		if (sTypes.find(std::string(tok.GetLex())) != sTypes.end())
			return sTypes[std::string(tok.GetLex())];
		Err("Cannot Parse Type '%s'!\n", std::string(tok.GetLex()).c_str());
		abort();
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
#pragma warning(push)
#pragma warning(disable: 4267)
			val += (view[i] - '0') * powi(10, view.size() - 1 - i);
#pragma warning(pop)
		}
		return val;
	}
}