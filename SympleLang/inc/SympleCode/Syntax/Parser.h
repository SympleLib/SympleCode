#pragma once

#include <vector>

#include "SympleCode/Syntax/Token.h"
#include "SympleCode/Syntax/Expression/ExpressionNode.h"
#include "SympleCode/Syntax/Expression/LiteralExpressionNode.h"

namespace Symple
{
	class Parser
	{
	private:
		std::vector<std::shared_ptr<Token>> mTokens;
		unsigned mPosition;
	public:
		Parser(char* file);
		Parser(std::vector<std::shared_ptr<Token>>& tokens);

		std::shared_ptr<Token> Peek(unsigned off = 0);
		std::shared_ptr<Token> Next();
		std::shared_ptr<Token> Match(Token::Kind);

		std::shared_ptr<ExpressionNode> ParseExpression();
		std::shared_ptr<ExpressionNode> ParsePrimaryExpression();
		std::shared_ptr<LiteralExpressionNode> ParseLiteralExpression();
	};
}