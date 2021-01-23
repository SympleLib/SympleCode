#include <pch.h>
#include "SympleCode/Lex/Token/Token.h"

namespace Symple
{
	Ref<Token> Token::Default = MakeRef<Token>(TokenKind::Unkown, "???", TokenInfo::Default, TokenInfo::Default);

	Token::Token(TokenKind kind, std::string_view text, Ref<TokenInfo> leadingInfo, Ref<TokenInfo> trailingInfo)
		: mKind(kind), mText(text), mLeadingInfo(leadingInfo), mTrailingInfo(trailingInfo)
	{}

	TokenKind Token::GetKind()
	{ return mKind; }

	std::string_view Token::GetText()
	{ return mText; }


	Ref<TokenInfo> Token::GetLeadingInfo()
	{ return mLeadingInfo; }

	Ref<TokenInfo> Token::GetTrailingInfo()
	{ return mTrailingInfo; }
}