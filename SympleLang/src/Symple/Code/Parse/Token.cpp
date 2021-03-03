#include "Symple/Code/Parse/Token.h"

namespace Symple::Code
{
	SYC_API std::ostream &operator <<(std::ostream &os, TokenKind kind)
	{ return os <<TokenKindNames[(uint32)kind]; }

	Token::Token(TokenKind kind, const char *beg, const char *end, const GlobalRef<const Symple::Code::File> &file, uint32 ln, uint32 disLn, uint32 col)
		: m_Kind(kind), m_Text(beg, end - beg), m_File(file), m_Line(ln), m_DisplayLine(disLn), m_Column(col - (end - beg)) {}


	bool Token::Is(TokenKind k) const
	{ return m_Kind == k; }


	void Token::Print(std::ostream &os, std::string indent, std::string_view label, bool last) const
	{
		PrintIndent(os, indent, label, last);
		os << '\"' << m_File->Name << "\" (" << m_DisplayLine << ':' << m_Column << ") [" << m_Kind << "] \'" << m_Text << '\'';
	}


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