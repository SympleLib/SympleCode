#pragma once

#include <vector>

#include "SympleCode/Syntax/Lexer.h"
#include "SympleCode/Syntax/Token.h"

#include "SympleCode/Syntax/Node.h"
#include "SympleCode/Syntax/ExpressionNode.h"
#include "SympleCode/Syntax/LiteralExpressionNode.h"

namespace Symple
{
	class Parser
	{
	private:
		std::vector<std::shared_ptr<Token>> mTokens;
		unsigned mPosition;

		std::shared_ptr<Lexer> mLexer;
	public:
		Parser(std::shared_ptr<Lexer>);
		Parser(std::shared_ptr<Lexer>, std::vector<std::shared_ptr<Token>>);

		Parser(std::vector<std::shared_ptr<Token>>);

		std::shared_ptr<Node> Parse();
		std::shared_ptr<ExpressionNode> ParseExpression();
		std::shared_ptr<LiteralExpressionNode> ParseLiteralExpression();
	private:
		std::shared_ptr<Token> Peek(unsigned off = 0);
		std::shared_ptr<Token> Next();
	};
}