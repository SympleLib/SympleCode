#pragma once

#include "Symple/Code/Util/Printable.h"
#include "Symple/Code/Util/File.h"
#include "Symple/Code/Util/Memory.h"

namespace Symple::Code
{
	enum class SYC_API TokenKind: uint32
	{
		Unknown,
		EndOfFile,

		Number,
		Identifier,

		// Punctuators
		Plus,
		Minus,
		Star,
		Slash,
		Percent,

		And,

		Equal,
		EqualArrow,

		OpenParen,
		CloseParen,
		OpenBrace,
		CloseBrace,

		Comma,
		Semicolon,

		// Keywords
		VoidKeyword,
		ByteKeyword,
		ShortKeyword,
		IntKeyword,
		LongKeyword,

		FloatKeyword,
		DoubleKeyword,
		TripleKeyword,

		CharKeyword,
		WCharKeyword,
		BoolKeyword,

		ReturnKeyword,

		// Markings
		Punctuator = Plus,
		Keyword = VoidKeyword,

		First = Unknown,
		Last = ReturnKeyword,
		Count, // Number of token kinds
	};

	constexpr const char *const TokenKindNames[(uint32)TokenKind::Count] =
	{
		"Unknown",
		"EndOfFile",

		"Number",
		"Identifier",


		"Plus",
		"Minus",
		"Star",
		"Slash",
		"Percent",

		"And",

		"Equal",
		"EqualArrow",

		"OpenParen",
		"CloseParen",
		"OpenBrace",
		"CloseBrace",

		"Comma",
		"Semicolon",


		"VoidKeyword",
		"ByteKeyword",
		"ShortKeyword",
		"IntKeyword",
		"LongKeyword",

		"FloatKeyword",
		"DoubleKeyword",
		"TripleKeyword",

		"CharKeyword",
		"WCharKeyword",
		"BoolKeyword",

		"ReturnKeyword",
	};

	SYC_API std::ostream &operator <<(std::ostream &, TokenKind);

	class SYC_API Token: public Printable
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

		virtual void Print(std::ostream &, std::string indent = "", std::string_view label = "", bool last = true) const override;

		TokenKind GetKind() const;
		std::string_view GetText() const;

		GlobalRef<const File> GetFile() const;
		uint32 GetLine() const;
		uint32 GetDisplayLine() const;
		uint32 GetColumn() const;

		SY_PROPERTY_GET(GetKind) TokenKind Kind;
		SY_PROPERTY_GET(GetText) std::string_view Text;

		SY_PROPERTY_GET(GetFile) GlobalRef<const File> File;
		SY_PROPERTY_GET(GetLine) uint32 Line;
		SY_PROPERTY_GET(GetDisplayLine) uint32 DisplayLine;
		SY_PROPERTY_GET(GetColumn) uint32 Column;
	};

	using TokenList = std::vector<GlobalRef<Token>>;
	using WeakTokenList = std::vector<WeakRef<Token>>;
	using ConstWeakTokenList = std::vector<WeakRef<const Token>>;
	using ConstTokenList = std::vector<GlobalRef<const Token>>;
}