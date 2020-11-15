#include "SympleCode/Parser.hpp"

#include <vector>
#include <iostream>

#include "SympleCode/Util/Util.hpp"

#include "SympleCode/Tree/Tree.hpp"

#include "SympleCode/Lexer.hpp"

namespace Symple::Parser
{
	static std::vector<Lexer::TokenInfo> sTokens;
	static std::vector<size_t> sStatements;
	static Tree sTree;
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
					if (sTokens[beg + 2].Is(Tokens::Exclimation) && sTokens[beg + 3].Is(Tokens::Equal))
					{
						Branch& ConditionOpTree = ConditionTree.PushBranch("Op. Not Equals");
						Branch& LValTree = ConditionOpTree.PushBranch("LVal");
						LValTree.PushBranch("Name", sTokens[beg + 1].GetLex());
						LValTree.PushBranch("Type", "int");
						Branch& RValTree = ConditionOpTree.PushBranch("RVal");
						RValTree.PushBranch("Type", "int");
						RValTree.PushBranch("Value", ParseInt(sTokens[beg + 4].GetLex()));
					}

					sCurrentTree = &WhileTree.PushBranch("Body");
				}
				else if (begin.GetLex() == "if")
				{
					Branch& WhileTree = sCurrentTree->PushBranch("If");

					Branch& ConditionTree = WhileTree.PushBranch("Condition");
					if (sTokens[beg + 2].Is(Tokens::Exclimation) && sTokens[beg + 3].Is(Tokens::Equal))
					{
						Branch& ConditionOpTree = ConditionTree.PushBranch("Op. Not Equals");
						Branch& LValTree = ConditionOpTree.PushBranch("LVal");
						LValTree.PushBranch("Name", sTokens[beg + 1].GetLex());
						LValTree.PushBranch("Type", "int");
						Branch& RValTree = ConditionOpTree.PushBranch("RVal");
						RValTree.PushBranch("Type", "int");
						RValTree.PushBranch("Value", ParseInt(sTokens[beg + 4].GetLex()));
					}
					else if (sTokens[beg + 2].Is(Tokens::GreaterThan))
					{
						Branch& ConditionOpTree = ConditionTree.PushBranch("Op. Greater Than");
						Branch& LValTree = ConditionOpTree.PushBranch("LVal");
						LValTree.PushBranch("Name", sTokens[beg + 1].GetLex());
						LValTree.PushBranch("Type", "int");
						Branch& RValTree = ConditionOpTree.PushBranch("RVal");
						RValTree.PushBranch("Name", sTokens[beg + 3].GetLex());
						RValTree.PushBranch("Type", "int");
					}

					sCurrentTree = &WhileTree.PushBranch("Body");
				}
			}
		}

		COut(sTree);
		Write(sTree);
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
}