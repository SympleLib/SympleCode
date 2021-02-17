#include "SympleCode/Syntax/Token.h"

#include <spdlog/spdlog.h>

#include "SympleCode/Syntax/Node.h"

namespace Symple::Syntax
{
	__SYC_API shared_ptr<Token> Token::Error = make_shared<Token>();
	__SYC_API shared_ptr<Token> Token::Default = make_shared<Token>();


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


	void Token::Print(std::ostream& os, std::string_view indent, bool last, std::string_view label)
	{
		Node::PrintIndent(os, indent, last, label);
		os << KindMap[GetKind()] << "Token '" << GetText() << "' <" << GetLine() << ':' << GetColumn() << ">";
	}

	void Token::PrintShort(std::ostream& os)
	{ os << '(' << KindMap[mKind] << ") " << mText; }


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