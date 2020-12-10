#pragma once

#include <vector>

#include "SympleCode/Lexer/Lexer.h"
#include "SympleCode/Common/Node/Node.h"
#include "SympleCode/Common/Node/ExpressionNode.h"
#include "SympleCode/Common/Node/CompilationUnitNode.h"

namespace Symple
{
	class Parser
	{
	private:
		Lexer mLexer;
		std::vector<const Token*> mTokens;
		size_t mPosition;
	public:
		Parser(const char* source = "");

		CompilationUnitNode* ParseCompilationUnit();
	private:
		const Token* Peek(size_t offset = 0);
		const Token* Next();
		const Token* Match(Token::Kind kind);
	public:
		ExpressionNode* ParseBinaryExpression(int parentPriority = -1);
		ExpressionNode* ParsePrimaryExpression();
	};
}