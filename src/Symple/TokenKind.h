/*
 * Copyright (c) 2022-present, Treidex <treidexy@gmail.com>
 *
 * MIT License
 */

#pragma once

#include <cstdint>

namespace Symple {
    enum class TokenKind : uint8_t {
        Unknown,

        Eof,
        Identifier,
        Integer,

		Count,
    };

	inline constexpr const char *tokenNames[(size_t) TokenKind::Count] = {
		"Unknown",

		"Eof",
		"Identifier",
		"Integer",
	};

    inline constexpr const char *getTokenName(TokenKind kind) {
		return tokenNames[(size_t) kind];
	}
}