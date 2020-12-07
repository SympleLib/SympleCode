#pragma once

#include "SympleCode/Lexer/Lexer.h"
#include "SympleCode/Common/Node/Node.h"

namespace Symple
{
	class Parser
	{
	private:
		Lexer mLexer;
		Token* mToken, mPToken;
	public:
		Parser(const char* source = "");

		Node* ParseCompilationUnit();
	private:
		Token* Next();
		Token* Match(Token::Kind kind);

		Node* ParseMembers();
		Node* ParseMember();

		Node* ParseExpresion();
		Node* ParseBinaryExpression();
	};
}