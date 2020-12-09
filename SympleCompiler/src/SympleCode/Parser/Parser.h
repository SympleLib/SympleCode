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
	private:
		const Token* Next();
		const Token* Match(Token::Kind kind);
	public:
		ExpressionNode* ParseBinaryExpression(int parentPrecedense = 0);
		ExpressionNode* ParsePrimaryExpression();
	};
}