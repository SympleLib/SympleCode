#include "Symple/Code/Parse/Token.h"

namespace Symple::Code
{
	Token::Token(TokenKind kind, const char *beg, const char *end, const GlobalRef<const Symple::Code::File> &file, uint32 ln, uint32 disLn, uint32 col)
		: m_Kind(kind), m_Text(beg, end - beg), m_File(file), m_Line(ln), m_DisplayLine(disLn), m_Column(col - (end - beg)) {}


	bool Token::Is(TokenKind k) const
	{ return m_Kind == k; }


	std::ostream &operator <<(std::ostream &os, const Token &tok)
	{ return os << '"' << tok.File->Name << "\" (" << tok.DisplayLine << ':' << tok.Column << ") [" << TokenKindNames[(uint32)tok.Kind] << "] '" << tok.Text << '\''; }

	std::ostream &operator <<(std::ostream &os, const GlobalRef<const Token> &tok)
	{
		if (tok.get())
			return os << *tok.get();
		else
			return os << "Null token";
	}

	std::ostream &operator <<(std::ostream &os, const WeakRef<const Token> &tok)
	{ return os << tok.lock(); }

	std::ostream &operator <<(std::ostream &os, const Scope<const Token> &tok)
	{ return os << *tok.get(); }


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