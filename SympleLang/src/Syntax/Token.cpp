#include "SympleCode/Syntax/Token.h"

#include <spdlog/spdlog.h>

#include "SympleCode/Syntax/Node.h"

namespace Symple::Syntax
{
	__SYC_API shared_ptr<Token> Token::Error = make_shared<Token>();
	__SYC_API shared_ptr<Token> Token::Default = make_shared<Token>();


	Token::Token(Kind kind, shared_ptr<Trivia> trivia, unsigned ln, unsigned col, char* file)
		: mKind(kind), mText(), mTrivia(trivia), mLine(ln), mColumn(col), mFile(file)
	{}

	Token::Token(Kind kind, std::string_view text, shared_ptr<Trivia> trivia, unsigned ln, unsigned col, char* file)
		: mKind(kind), mText(text), mTrivia(trivia), mLine(ln), mColumn(col), mFile(file)
	{}

	Token::Token(Kind kind, char* beg, unsigned len, shared_ptr<Trivia> trivia, unsigned ln, unsigned col, char* file)
		: mKind(kind), mText(beg, len), mTrivia(trivia), mLine(ln), mColumn(col), mFile(file)
	{}

	Token::Token(Kind kind, char* beg, char* end, shared_ptr<Trivia> trivia, unsigned ln, unsigned col, char* file)
		: mKind(kind), mText(beg, std::distance(beg, end)), mTrivia(trivia), mLine(ln), mColumn(col), mFile(file)
	{}


	bool Token::IsKeyword()
	{ return mKind >= FirstKeyword; }

	bool Token::Is(Kind kind)
	{ return mKind == kind; }


	void Token::Print(std::ostream& os, std::string_view indent, bool last, std::string_view label)
	{
		Node::PrintIndent(os, indent, last, label);
		os << KindMap[GetKind()] << "Token '" << GetText() << "' ";
		GetTrivia()->PrintShort(os);
		os << " <" << GetLine() << ':' << GetColumn() << ">";
	}

	void Token::PrintShort(std::ostream& os)
	{ os << '(' << KindMap[mKind] << ") " << mText; }


	Token::Kind Token::GetKind()
	{ return mKind; }

	std::string_view Token::GetText()
	{ return mText; }

	shared_ptr<Trivia> Token::GetTrivia()
	{ return mTrivia; }


	char* Token::GetFile()
	{ return mFile; }

	unsigned Token::GetLine()
	{ return mLine; }

	unsigned Token::GetColumn()
	{ return mColumn; }
}