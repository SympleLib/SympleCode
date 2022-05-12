/*
 * Copyright (c) 2022-present, Treidex <treidexy@gmail.com>
 *
 * MIT License
 */

#pragma once

#include "syc/SourceRange.h"
#include "syc/TokenKind.h"

#include <string>
#include <string_view>

namespace syc {
	using TokenFlags = uint8_t;
	namespace TokenFlag {
		enum: uint8_t {
			None = 0,

			StartOfLine = 1 << 0,
			LeadingSpace = 1 << 1,
		};
	}

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
			return getTokenName(kind);
		}

		std::string getFlagsAsString() const;
    };
} // syc
