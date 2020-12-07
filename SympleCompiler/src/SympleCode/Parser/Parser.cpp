#include "Parser.h"

#include "SympleCode/Common/Node/CompilationUnitNode.h"

namespace Symple
{
	Parser::Parser(const char* source = "")
		: mLexer(source)
	{}

	Token* Parser::Next()
	{
		mPToken = mToken;
		mToken = mLexer.Next();
		return mPToken;
	}

	Token* Parser::Match(Token::Kind kind)
	{
		if (mPToken.Is(kind))
			return Next();
	}

	Node* Parser::ParseCompilationUnit()
	{
		Node* members = ParseMembers();

		return compilationUnit;
	}
}