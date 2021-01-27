#pragma once

#include <memory>
#include <iostream>
#include <string_view>

namespace Symple
{
	using std::shared_ptr;
	using std::make_shared;

	using std::unique_ptr;
	using std::make_unique;
}

namespace Symple::Syntax
{
	class Token
	{
	public: enum Kind : unsigned;
	private:
		Kind mKind;
		std::string_view mText;

		char* mFile;
		unsigned mLine, mColumn;
	public:
		Token(Kind = Unknown, unsigned ln = 0, unsigned col = 0, char* file = "<NA>");
		Token(Kind, std::string_view text, unsigned ln = 0, unsigned col = 0, char* file = "<NA>");
		Token(Kind, char* beg, unsigned len = 1, unsigned ln = 0, unsigned col = 0, char* file = "<NA>");
		Token(Kind, char* beg, char* end, unsigned ln = 0, unsigned col = 0, char* file = "<NA>");

		bool Is(Kind kind);
		template <typename... Args>
		bool IsEither(Kind kind, Args... kinds)
		{ return Is(kind) || IsEither(kinds...); }

		void Print(std::ostream& = std::cout);
		void PrintShort(std::ostream& = std::cout);

		Kind GetKind();
		std::string_view GetText();

		char* GetFile();
		unsigned GetLine();
		unsigned GetColumn();
	public:
		enum Kind : unsigned
		{
			EndOfFile,
			Unknown,

			Identifier,
			Number,

			Semicolon,

			Plus,
			Dash,
			Asterisk,
			Slash,

			Equal,

			OpenParenthesis,
			CloseParenthesis,

			Last = CloseParenthesis,
		};

		static constexpr char* KindMap[Last + 1] = {
			"EndOfFile",
			"Unknown",

			"Identifier",
			"Number",

			"Semicolon",

			"Plus",
			"Dash",
			"Asterisk",
			"Slash",

			"Equal",

			"OpenParenthesis",
			"CloseParenthesis",
		};
	};
}