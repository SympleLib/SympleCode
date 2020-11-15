#include "SympleCode/Parser.hpp"

#include <vector>
#include <iostream>

#include "SympleCode/Tree/Tree.hpp"

#include "SympleCode/Lexer.hpp"

#define Out(...) fprintf(stdout, __VA_ARGS__)
#define Err(...) fprintf(stderr, __VA_ARGS__)

#define COut(msg) std::cout << msg;
#define CErr(msg) std::cerr << msg;

namespace Parser
{
	static std::vector<Lexer::TokenInfo> sTokens;
	static std::vector<size_t> sStatements;
	static Branch sTree;
	static Branch* sCurrentTree;

	static bool myLex(const Lexer::TokenInfo& tokenInfo)
	{
		if (tokenInfo.Is(Tokens::Semicolon))
			sStatements.push_back(sTokens.size());
		else
		{
			if (tokenInfo.IsEither(Tokens::End, Tokens::Unexpected))
			{
				if (tokenInfo.Is(Tokens::Unexpected))
					CErr("Unexpected Token: '" << tokenInfo.GetLex() << "'\n");

				return false;
			}

			sTokens.push_back(tokenInfo);
			//COut('\'' << tokenInfo.GetLex() << "'\n");
		}
		return true;
	}

	void Parse(const char* beg)
	{
		Lexer::Lex(beg, myLex);
		sCurrentTree = &(sTree = {});

		for (size_t s = 0; s < sStatements.size(); s++)
		{
			size_t beg = s > 0 ? sStatements[s - 1] : 0;
			size_t end = sStatements[s];
			
			Lexer::TokenInfo& begin = sTokens[beg];
			if (begin.Is(Tokens::Identifier))
			{
				if (begin.GetLex() == "int")
				{
					Branch& VarTree = sCurrentTree->PushBranch("Var Dec");
					VarTree.PushBranch("Name", sTokens[beg + 1].GetLex());
					VarTree.PushBranch("Type", sTokens[beg].GetLex());
				}
				else if (begin.GetLex() == "while")
				{
					Branch& WhileTree = sCurrentTree->PushBranch("While Loop");
					Branch& ConditionTree = WhileTree.PushBranch("Condition");
					Branch& BodyTree = WhileTree.PushBranch("Body");
				}
			}
		}

		Out(sTree.ToString().c_str());
	}

#ifdef WIN32
	int32_t ParseInt(const std::string_view& view)
#else
	int64_t ParseInt(const std::string_view& view)
#endif
	{
		int val = 0;
		for (size_t i = 0; i < view.size(); i++)
		{
			val += (view[i] - '0') * powi(10, view.size() - 1 - i);
		}
		return val;
	}

	int powi(int v, int t)
	{
		int val = 1;
		for (size_t i = 0; i < t; i++)
			val *= v;
		return val;
	}
}