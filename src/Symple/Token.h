/*
 * Copyright (c) 2022-present, Treidex <treidexy@gmail.com>
 *
 * MIT License
 */

#pragma once

#include "Symple/SourceRange.h"
#include "Symple/TokenKind.h"

#include <string>
#include <string_view>

namespace Symple {
    class Token {
    public:
        TokenKind kind;
		std::string text;
        SourceRange sourceRange;

    public:
        Token() = default;
        Token(TokenKind kind, std::string &&text, SourceRange sourceRange);
        Token(TokenKind kind, const std::string &text, SourceRange sourceRange);
        Token(TokenKind kind, std::string_view text, SourceRange sourceRange);

        bool is(TokenKind k) const
        { return kind == k; }

        template<typename...T>
        bool isOneOf(TokenKind k, T... ks) const
        { return is(k) || isOneOf(ks...); }

		const char *getName() const {
			return getTokenName(kind);
		}
    };
} // Symple
