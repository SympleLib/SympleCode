#pragma once

#include <iostream>
#include <string_view>

#include "SympleCode/Memory.h"

namespace Symple::Syntax
{
	class __SYC_API Trivia
	{
	public: typedef unsigned Kind;
	private:
		Kind mKind;
		std::string mText;

		char* mFile;
		unsigned mLine, mColumn;
	public:
		Trivia(Kind = Unknown, unsigned ln = 0, unsigned col = 0, char* file = "<NA>");
		Trivia(Kind, std::string_view text, unsigned ln = 0, unsigned col = 0, char* file = "<NA>");
		Trivia(Kind, char* beg, unsigned len = 1, unsigned ln = 0, unsigned col = 0, char* file = "<NA>");
		Trivia(Kind, char* beg, char* end, unsigned ln = 0, unsigned col = 0, char* file = "<NA>");

		bool Is(Kind kind);
		template <typename... Args>
		bool Is(Kind kind, Args... kinds)
		{ return Is(kind) || Is(kinds...); }

		void Print(std::ostream& = std::cout, std::string_view indent = "", bool last = true, std::string_view label = "");
		void PrintShort(std::ostream& = std::cout);

		Kind GetKind();
		std::string_view GetText();

		char* GetFile();
		unsigned GetLine();
		unsigned GetColumn();

		static shared_ptr<Trivia> Error;
		static shared_ptr<Trivia> Default;
	public:
		enum _Kind : unsigned
		{
			Unknown = 0 << 0,

			StartOfLine  = 1 << 0,
			LeadingSpace = 1 << 1,

			Length = 3,
		};

		static constexpr Kind KindArray[Length] = {
			Unknown,

			StartOfLine,
			LeadingSpace,
		};

		static constexpr char* KindMap[Length] = {
			"Unknown",

			"StartOfLine",
			"LeadingSpace",
		};
	};
}