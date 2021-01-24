#pragma once

#include <memory>
#include <string_view>

namespace Symple
{
	class Token
	{
	public: enum Kind;
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

		Kind GetKind();
		std::string_view GetText();

		char* GetFile();
		unsigned GetLine();
		unsigned GetColumn();
	public:
		enum Kind
		{
			EndOfFile,
			Unknown,

			Identifier,
			Number,

			Semicolon,

			Equal,

			Last = Equal,
		};

		static constexpr char* KindMap[(int)Last + 1] = {
			"EndOfFile",
			"Unknown",

			"Identifier",
			"Number",

			"Semicolon",

			"Equal",
		};
	};
}