/*
 * Copyright (c) 2022-present, Treidex <treidexy@gmail.com>
 *
 * MIT License
 */

#pragma once

#include "syc/SourceLocation.h"
#include "syc/FunEnum.h"

#include <string>
#include <string_view>

namespace syc {
	using TokenFlags = uint8_t;

	enum class TokenKind : uint8_t {
#define TOKEN(x) x,
#include "syc/parse/TokenKind.h"
#undef TOKEN

		Count,
	};

	inline constexpr const char *TokenKindNames[(uint64_t) TokenKind::Count] = {
#define TOKEN(x) #x " ",
#define PUNCTUATOR(x, y) #x " '" y "'",
#include "syc/parse/TokenKind.h"
	};

	ENUM_NAME_FUNC(TokenKind);

    class Token {
    public:
        TokenKind kind;
		std::string text;
        SourceRange sourceRange;
		TokenFlags flags;

    public:
        Token() = default;
        Token(TokenKind kind, std::string &&text, SourceRange sourceRange, TokenFlags flags);
        Token(TokenKind kind, const std::string &text, SourceRange sourceRange, TokenFlags flags);
        Token(TokenKind kind, std::string_view text, SourceRange sourceRange, TokenFlags flags);

        bool is(TokenKind k) const
        { return kind == k; }

        template<typename...T>
        bool isOneOf(TokenKind k, T... ks) const
        { return is(k) || isOneOf(ks...); }

		const char *getName() const {
			return getTokenKindName(kind);
		}

		std::string getFlagsAsString() const;
    };

	namespace TokenFlag {
		enum: TokenFlags {
			None = 0,

			StartOfLine = 1 << 0,
			LeadingSpace = 1 << 1,
		};
	}
} // syc
