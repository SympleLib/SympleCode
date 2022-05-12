/*
 * Copyright (c) 2022-present, Treidex <treidexy@gmail.com>
 *
 * MIT License
 */

#pragma once

#include <cstdint>

namespace syc {
    enum class TokenKind : uint8_t {
        Unknown,

		// Unique
        Eof,
        Identifier,
        Integer,

		// Punctuators
		Equal,
		Semicolon,
		Plus,
		Minus,

		Count,
    };

	inline constexpr const char *tokenNames[(size_t) TokenKind::Count] = {
		"Unknown",

		"Eof",
		"Identifier",
		"Integer",

		"Equal",
		"Semicolon",
		"Plus",
		"Minus",
	};

    inline constexpr const char *getTokenName(TokenKind kind) {
		return tokenNames[(size_t) kind];
	}
}