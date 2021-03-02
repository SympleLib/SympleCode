#pragma once

#include "Symple/Code/Util/File.h"
#include "Symple/Code/Memory.h"

namespace Symple::Code
{
	enum class SYC_API TokenKind: uint32
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
		"Slash",
		"Percent",

		"SingleLineComment",
		"MultiLineComment",
	};

	class SYC_API Token
	{
	private:
		TokenKind m_Kind;
		std::string_view m_Text;

		const GlobalRef<const File> m_File;
		// Line, Display Line, Column
		uint32 m_Line, m_DisplayLine, m_Column;
	public:
		Token(TokenKind, const char *begin, const char *end, const GlobalRef<const File> &, uint32 line, uint32 displayLine, uint32 column);

		bool Is(TokenKind) const;
		template <typename... Args>
		bool Is(TokenKind kind, Args&&... kinds) const
		{ return Is(kind) || Is(kinds...); }

		TokenKind GetKind() const;
		std::string_view GetText() const;

		GlobalRef<const File> GetFile() const;
		uint32 GetLine() const;
		uint32 GetDisplayLine() const;
		uint32 GetColumn() const;

		__declspec(property(get = GetKind)) TokenKind Kind;
		SY_PROPERTY_GET(GetText) std::string_view Text;

		SY_PROPERTY_GET(GetFile) GlobalRef<const File> File;
		SY_PROPERTY_GET(GetLine) uint32 Line;
		SY_PROPERTY_GET(GetDisplayLine) uint32 DisplayLine;
		SY_PROPERTY_GET(GetColumn) uint32 Column;
	};

	using TokenList = std::vector<GlobalRef<Token>>;
}