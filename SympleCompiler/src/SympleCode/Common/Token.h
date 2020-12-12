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

			Plus,
			Minus,
			Asterisk,
			Slash,
			Equal,

			Semicolon,

			OpenBracket,
			CloseBracket,
			OpenParenthesis,
			CloseParenthesis,

			True,
			False,

			Void,

			Function,

			Unknown = -2,
			EndOfFile = -1,
		};

		static constexpr const char* KindMap[] = {
			"Identifier", "Number",
			"Plus", "Minus", "Asterisk", "Slash", "Equal",
			"Semicolon",
			"OpenBracket", "CloseBracket", "OpenParenthesis", "CloseParenthesis",
			"True", "False",
			"Void",
			"Function",
		};

		static constexpr const char* KindString(Kind kind);
	private:
		Kind mKind;
		std::string_view mLex;
	public:
		Token(Kind kind = Kind::Unknown);
		Token(Kind kind, const char* beg, size_t len);
		Token(Kind kind, const char* beg, const char* end);

		bool Is(Kind kind) const;
		bool IsEither(std::initializer_list<Kind> kinds) const;

		const Kind& GetKind() const;
		const std::string_view& GetLex() const;
	};
}