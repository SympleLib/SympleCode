#include "SympleCode/Syntax/Trivia.h"

#include "SympleCode/Syntax/Node.h"

namespace Symple::Syntax
{
	__SYC_API shared_ptr<Trivia> Trivia::Error = make_shared<Trivia>();
	__SYC_API shared_ptr<Trivia> Trivia::Default = make_shared<Trivia>();


	Trivia::Trivia(Kind kind, unsigned ln, unsigned col, char* file)
		: mKind(kind), mText(), mLine(ln), mColumn(col), mFile(file)
	{}

	Trivia::Trivia(Kind kind, std::string_view text, unsigned ln, unsigned col, char* file)
		: mKind(kind), mText(text), mLine(ln), mColumn(col), mFile(file)
	{}

	Trivia::Trivia(Kind kind, char* beg, unsigned len, unsigned ln, unsigned col, char* file)
		: mKind(kind), mText(beg, len), mLine(ln), mColumn(col), mFile(file)
	{}

	Trivia::Trivia(Kind kind, char* beg, char* end, unsigned ln, unsigned col, char* file)
		: mKind(kind), mText(beg, std::distance(beg, end)), mLine(ln), mColumn(col), mFile(file)
	{}


	bool Trivia::Is(Kind kind)
	{ return mKind & kind; }


	void Trivia::Print(std::ostream& os, std::string_view indent, bool last, std::string_view label)
	{
		Node::PrintIndent(os, indent, last, label);
		for (unsigned k = 0; k < Length; k++)
			if (Is(KindArray[k]))
				os << '[' << KindMap[KindArray[k]] << "] ";

		os << "Trivia '" << GetText() << "' <" << GetLine() << ':' << GetColumn() << ">";
	}

	void Trivia::PrintShort(std::ostream& os)
	{
		for (unsigned k = 0; k < Length; k++)
			if (Is(KindArray[k]))
				os << '[' << KindMap[KindArray[k]] << "] ";
	}


	Trivia::Kind Trivia::GetKind()
	{ return mKind; }

	std::string_view Trivia::GetText()
	{ return mText; }


	char* Trivia::GetFile()
	{ return mFile; }

	unsigned Trivia::GetLine()
	{ return mLine; }

	unsigned Trivia::GetColumn()
	{ return mColumn; }
}