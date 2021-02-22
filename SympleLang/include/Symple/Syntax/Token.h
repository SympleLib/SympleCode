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
		// Line, Display Line, Column
		uint32 ln, disLn, col;
	public:
		Token(TokenKind, const char *begin, const char *end, const GlobalRef<File>&, uint32 line, uint32 displayLine, uint32 column);

		TokenKind GetKind() const;
		std::string_view GetText() const;

		GlobalRef<File> GetFile() const;
		uint32 GetLine() const;
		uint32 GetDisplayLine() const;
		uint32 GetColumn() const;

		SY_PROPERTY_GET(GetKind) TokenKind Kind;
		SY_PROPERTY_GET(GetText) std::string_view Text;

		SY_PROPERTY_GET(GetFile) GlobalRef<File> File;
		SY_PROPERTY_GET(GetLine) uint32 Line;
		SY_PROPERTY_GET(GetDisplayLine) uint32 DisplayLine;
		SY_PROPERTY_GET(GetColumn) uint32 Column;
	};
}