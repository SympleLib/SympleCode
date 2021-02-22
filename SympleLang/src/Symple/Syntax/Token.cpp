#include "Symple/Common/Common.h"
#include "Symple/Syntax/Token.h"

namespace Symple
{
	Token::Token(TokenKind kind, const char *beg, const char *end, const GlobalRef<Symple::File> &file, uint32 ln, uint32 disLn, uint32 col)
		: kind(kind), text(beg, end - beg), file(file), ln(ln), disLn(disLn), col(col) {}


	bool Token::Is(TokenKind k) const
	{ return kind == k; }


	TokenKind Token::GetKind() const
	{ return kind; }

	std::string_view Token::GetText() const
	{ return text; }


	GlobalRef<File> Token::GetFile() const
	{ return file; }

	uint32 Token::GetLine() const
	{ return ln; }

	uint32 Token::GetDisplayLine() const
	{ return disLn; }

	uint32 Token::GetColumn() const
	{ return col; }
}