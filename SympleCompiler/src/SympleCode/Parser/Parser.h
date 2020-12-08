#pragma once

#include "SympleCode/Lexer/Lexer.h"
#include "SympleCode/Common/Node/Node.h"

namespace Symple
{
	class Parser
	{
	private:
		Lexer mLexer;
		const Token* mToken;
		const Token* mPToken;
	public:
		Parser(const char* source = "");

		Node* ParseCompilationUnit();
	private:
		const Token* Next();
		const Token* Match(Token::Kind kind);

		const std::list<Node*> ParseMembers();
		Node* ParseMember();

		Node* ParseGlobalStatement();
		Node* ParseStatement();

		Node* ParseExpresion();
		Node* ParseAssignmentExpresion();
		Node* ParseBinaryExpression(int parentPrecedense = 0);
		Node* ParsePrimaryExpression();

		Node* ParseNumberLiteral();
	};
}