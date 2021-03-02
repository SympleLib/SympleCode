#include "Symple/Code/Parse/Token.h"

namespace Symple::Code
{
	Token::Token(TokenKind kind, const char *beg, const char *end, const GlobalRef<const Symple::Code::File> &file, uint32 ln, uint32 disLn, uint32 col)
		: m_Kind(kind), m_Text(beg, end - beg), m_File(file), m_Line(ln), m_DisplayLine(disLn), m_Column(col) {}


	bool Token::Is(TokenKind k) const
	{ return m_Kind == k; }


	TokenKind Token::GetKind() const
	{ return m_Kind; }

	std::string_view Token::GetText() const
	{ return m_Text; }


	GlobalRef<const File> Token::GetFile() const
	{ return m_File; }

	uint32 Token::GetLine() const
	{ return m_Line; }

	uint32 Token::GetDisplayLine() const
	{ return m_DisplayLine; }

	uint32 Token::GetColumn() const
	{ return m_Column; }
}