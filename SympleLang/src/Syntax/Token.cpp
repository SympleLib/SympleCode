#include "SympleCode/Syntax/Token.h"

namespace Symple
{
	Token::Token(Kind kind, unsigned ln, unsigned col, char* file)
		: mKind(kind), mText(), mLine(ln), mColumn(col), mFile(file)
	{}

	Token::Token(Kind kind, std::string_view text, unsigned ln, unsigned col, char* file)
		: mKind(kind), mText(text), mLine(ln), mColumn(col), mFile(file)
	{}

	Token::Token(Kind kind, char* beg, unsigned len, unsigned ln, unsigned col, char* file)
		: mKind(kind), mText(beg, len), mLine(ln), mColumn(col), mFile(file)
	{}

	Token::Token(Kind kind, char* beg, char* end, unsigned ln, unsigned col, char* file)
		: mKind(kind), mText(beg, std::distance(beg, end)), mLine(ln), mColumn(col), mFile(file)
	{}


	bool Token::Is(Kind kind)
	{ return mKind == kind; }


	Token::Kind Token::GetKind()
	{ return mKind; }

	std::string_view Token::GetText()
	{ return mText; }


	char* Token::GetFile()
	{ return mFile; }

	unsigned Token::GetLine()
	{ return mLine; }

	unsigned Token::GetColumn()
	{ return mColumn; }

}