#pragma once

#include <string_view>
#include <initializer_list>

namespace Symple
{
	class Token
	{
	public:
		enum class Kind
		{
			Identifier,
			Number,
			String,

			Plus,
			Minus,
			Asterisk,
			Slash,
			Equal,

			EqualEqual,
			NotEqual,

			Comma,
			Semicolon,

			OpenBracket,
			CloseBracket,
			OpenParenthesis,
			CloseParenthesis,

			LeftArrow,
			RightArrow,

			Hint,
			True,
			False,
			While,
			Break,
			Return,
			Extern,

			If,
			Else,

			Comment,

			Unknown = -2,
			EndOfFile = -1,
		};

		static constexpr const char* KindMap[] = {
			"Identifier", "Number", "String",
			"Plus", "Minus", "Asterisk", "Slash", "Equal",
			"EqualEqual", "NotEqual"
			"Comma", "Semicolon",
			"OpenBracket", "CloseBracket", "OpenParenthesis", "CloseParenthesis",
			"LeftArrow", "RightArrow",
			"Hint", "True", "False", "While", "Break", "Return", "Extern",
			"If", "Else",
			"Comment",
		};

		static const char* KindString(Kind kind);
	private:
		Kind mKind;
		std::string_view mLex;

		int mLine, mColumn;
	public:
		Token(Kind kind = Kind::Unknown, int line = 0, int column = 0);
		Token(Kind kind, const char* beg, size_t len, int line = 0, int column = 0);
		Token(Kind kind, const char* beg, const char* end, int line = 0, int column = 0);

		bool Is(Kind kind) const;
		bool IsEither(std::initializer_list<Kind> kinds) const;

		const Kind& GetKind() const;
		const std::string_view& GetLex() const;
		int GetLine() const;
		int GetColumn() const;
	};
}