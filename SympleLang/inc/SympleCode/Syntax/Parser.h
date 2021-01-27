#pragma once

#include <vector>

#include "SympleCode/Syntax/Lexer.h"
#include "SympleCode/Syntax/Token.h"

#include "SympleCode/Syntax/Node.h"
#include "SympleCode/Syntax/ExpressionSyntax.h"
#include "SympleCode/Syntax/UnaryExpressionSyntax.h"
#include "SympleCode/Syntax/BinaryExpressionSyntax.h"
#include "SympleCode/Syntax/LiteralExpressionSyntax.h"
#include "SympleCode/Syntax/ParenthesizedExpressionSyntax.h"

#include "SympleCode/DiagnosticBag.h"

namespace Symple::Syntax
{
	class Parser
	{
	private:
		std::vector<shared_ptr<Token>> mTokens;
		unsigned mPosition = 0;

		shared_ptr<Lexer> mLexer;
		shared_ptr<DiagnosticBag> mDiagnosticBag = make_shared<DiagnosticBag>();
	public:
		Parser(shared_ptr<Lexer>);
		Parser(shared_ptr<Lexer>, std::vector<shared_ptr<Token>>);

		Parser(std::vector<shared_ptr<Token>>);

		shared_ptr<Syntax::Node> Parse();
		shared_ptr<ExpressionSyntax> ParseExpression();
		shared_ptr<ExpressionSyntax> ParseUnaryExpression(unsigned parentPrecedence = 0);
		shared_ptr<ExpressionSyntax> ParseBinaryExpression(unsigned parentPrecedence = 0);

		shared_ptr<ExpressionSyntax> ParsePrimaryExpression();
		shared_ptr<LiteralExpressionSyntax> ParseLiteralExpression();
		shared_ptr<ParenthesizedExpressionSyntax> ParseParenthesizedExpression();

		shared_ptr<DiagnosticBag> GetDiagnosticBag();
	private:
		shared_ptr<Token> Peek(unsigned off = 0);
		shared_ptr<Token> Next();
		shared_ptr<Token> Match(Token::Kind);
	};
}