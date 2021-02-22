#include "Symple/Common/Common.h"
#include "Symple/Syntax/Token.h"

namespace Symple
{
	Token::Token(TokenKind kind, std::string_view text, const GlobalRef<Symple::File> &file)
		: kind(kind), text(text), file(file) {}

	TokenKind Token::GetKind() const
	{ return kind; }

	std::string_view Token::GetText() const
	{ return text; }

	GlobalRef<File> Token::GetFile() const
	{ return file; }
}