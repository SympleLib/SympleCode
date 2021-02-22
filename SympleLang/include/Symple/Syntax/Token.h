#pragma once

#include "Symple/Common/Common.h"

namespace Symple
{
	enum class TokenKind: uint32
	{
		Unknown,
		EndOfFile,

		Number,
		Identifier,

		Plus,
		Dash,
		Star,
		Slash,
		Percent,

		SingleLineComment,
		MultiLineComment,

		// Markings
		Punctuator = Plus,
		Comment = SingleLineComment,

		First = Unknown,
		Last = MultiLineComment,
		Count, // Number of token kinds
	};

	constexpr const char *const TokenKindNames[(uint32)TokenKind::Count] =
	{
		"Unknown",
		"EndOfFile",

		"Number",
		"Identifier",

		"Plus",
		"Dash",
		"Star",
		"Percent",

		"SingleLineComment",
		"MultiLineComment",
	};

	class SY_API Token
	{
	private:
		TokenKind kind;
		std::string_view text;

		GlobalRef<File> file;
	public:
		Token(TokenKind, const char *begin, const char *end, const GlobalRef<File>&);

		TokenKind GetKind() const;
		std::string_view GetText() const;
		GlobalRef<File> GetFile() const;

		SY_PROPERTY_GET(GetKind) TokenKind Kind;
		SY_PROPERTY_GET(GetText) std::string_view Text;
		SY_PROPERTY_GET(GetFile) GlobalRef<File> File;
	};
}