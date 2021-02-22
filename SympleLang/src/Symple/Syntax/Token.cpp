#include "Symple/Common/Common.h"
#include "Symple/Syntax/Token.h"

namespace Symple
{
	Token::Token(TokenKind kind, const char *beg, const char *end, const GlobalRef<Symple::File> &file)
		: kind(kind), text(beg, end - beg), file(file) {}

	TokenKind Token::GetKind() const
	{ return kind; }

	std::string_view Token::GetText() const
	{ return text; }

	GlobalRef<File> Token::GetFile() const
	{ return file; }
}