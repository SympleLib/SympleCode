#include "Symple/Code/Parse/Lexer.h"

namespace Symple::Code
{
	Lexer::Lexer(const GlobalRef<const File> &file)
		: m_File(file) {}

	TokenList Lexer::LexAll()
	{
		TokenList toks;

		do
			toks.push_back(Lex());
		while (Current);
		return toks;
	}

	GlobalRef<Token> Lexer::Lex()
	{
		if (!Current)
		{
			const char *beg = &Next();
			return MakeToken(TokenKind::EndOfFile, beg, &Current);
		}
	}
}