#include "Symple/Common/Common.h"
#include "Symple/Syntax/Lexer.h"

namespace Symple
{
	Lexer::Lexer(const GlobalRef<File> &file)
		: file(file), p(file->Source.c_str()) {}


	GlobalRef<Token> Lexer::Lex()
	{
		
	}


	const char &Lexer::Peek(uint32 off)
	{
		const char *c = p + off;
		if (c > file->Source.cend().operator->())
			return *file->Source.cend().operator->();
		return *c;
	}

	const char &Lexer::Next()
	{
		const char &prev = Current;
		col++;
		p++;
		assert(p <= file->Source.cend().operator->());
		return prev;
	}

	static int8 NumberPrefix(const std::string &str)
	{
		str.starts_with("0");
	}

	static bool IsNumber(char)
	{

	}

	static bool IsIdentifier(char)
	{

	}

	static bool IsWhiteSpace(char)
	{
		
	}
}