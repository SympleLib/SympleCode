#pragma once

#include <memory>
#include <string_view>

#include "SympleCode/Ref.h"

#include "SympleCode/Lex/Token/TokenInfo.h"
#include "SympleCode/Lex/Token/TokenKind.h"

namespace Symple
{
	class Token
	{
	private:
		TokenKind mKind;
		std::string_view mText;

		Ref<TokenInfo> mLeadingInfo;  // Info at beginning of token
		Ref<TokenInfo> mTrailingInfo; // Info at end of token
	public:
		static Ref<Token> Default;

		Token(TokenKind kind = Default->mKind, std::string_view text = Default->mText, Ref<TokenInfo> leadingInfo = Default->mLeadingInfo, Ref<TokenInfo> trailingInfo = Default->mTrailingInfo);

		TokenKind GetKind();
		std::string_view GetText();

		Ref<TokenInfo> GetLeadingInfo();
		Ref<TokenInfo> GetTrailingInfo();
	};
}