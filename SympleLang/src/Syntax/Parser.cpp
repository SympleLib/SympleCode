#include "SympleCode/Syntax/Parser.h"

#include "SympleCode/Syntax/Lexer.h"

namespace Symple
{
	Parser::Parser(char* file)
		: mPosision()
	{
		Lexer lexer(file);
		std::shared_ptr<Token> tok = std::make_shared<Token>();
		while (!tok->Is(Token::EndOfFile))
			mTokens.push_back(tok = lexer.Lex());
	}

	Parser::Parser(std::vector<std::shared_ptr<Token>>& tokens)
		: mPosision(), mTokens(tokens)
	{}
}