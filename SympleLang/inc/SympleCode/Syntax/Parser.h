#pragma once

#include <vector>

#include "SympleCode/Syntax/Lexer.h"
#include "SympleCode/Syntax/Token.h"

#include "SympleCode/Syntax/Node.h"
#include "SympleCode/Syntax/ExpressionNode.h"
#include "SympleCode/Syntax/UnaryExpressionNode.h"
#include "SympleCode/Syntax/BinaryExpressionNode.h"
#include "SympleCode/Syntax/LiteralExpressionNode.h"

namespace Symple::Syntax
{
	class Parser
	{
	private:
		std::vector<shared_ptr<Token>> mTokens;
		unsigned mPosition;

		shared_ptr<Lexer> mLexer;
	public:
		Parser(shared_ptr<Lexer>);
		Parser(shared_ptr<Lexer>, std::vector<shared_ptr<Token>>);

		Parser(std::vector<shared_ptr<Token>>);

		shared_ptr<Node> Parse();
		shared_ptr<ExpressionNode> ParseExpression();
		shared_ptr<ExpressionNode> ParseUnaryExpression(unsigned parentPrecedence = 0);
		shared_ptr<ExpressionNode> ParseBinaryExpression(unsigned parentPrecedence = 0);
		shared_ptr<LiteralExpressionNode> ParseLiteralExpression();
	private:
		shared_ptr<Token> Peek(unsigned off = 0);
		shared_ptr<Token> Next();
	};
}