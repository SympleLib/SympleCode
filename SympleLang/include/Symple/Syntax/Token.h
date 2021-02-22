#pragma once

#include "Symple/Common/Common.h"

namespace Symple
{
	enum class TokenKind: uint8
	{
		EndOfFile,

		Number,
		Identifier,

		// Markings
		First = EndOfFile,
		Last = Identifier,
		Count, // Number of token kinds
	};

	constexpr const char *const TokenKindNames[(uint8)TokenKind::Count] =
	{
		"EndOfFile",

		"Number",
		"Identifier",
	};

	class SY_API Token
	{
	private:
		TokenKind kind;
		std::string_view text;

		GlobalRef<File> file;
	public:
		Token(TokenKind, std::string_view text, const GlobalRef<File>&);

		TokenKind GetKind() const;
		std::string_view GetText() const;
		GlobalRef<File> GetFile() const;

		SY_PROPERTY_GET(GetKind) TokenKind Kind;
		SY_PROPERTY_GET(GetText) std::string_view Text;
		SY_PROPERTY_GET(GetFile) GlobalRef<File> File;
	};
}