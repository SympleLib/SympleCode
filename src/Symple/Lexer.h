/*
 * Copyright (c) 2022-present, Treidex <treidexy@gmail.com>
 *
 * MIT License
 */

#pragma once

#include "Symple/Token.h"
#include <string_view>
#include <vector>

namespace Symple {
    class Lexer {
	private:
		std::vector<Token> tokens;

		size_t pos;
		size_t line, column;

		std::string_view source;
		Position start;
		size_t startPos;

	public:
		Lexer() = default;
		std::vector<Token> lex(std::string_view source);

	private:
		void lexToken();
		void skipWhitespace();

		void lexIdentifierOrKeyword();

		void lexNumberLiteral();
		void lexBinaryLiteral();
		void lexOctalLiteral();
		void lexDecimalLiteral();
		void lexHexLiteral();

		char next();
		char peek(ssize_t offset = 0) const;

		Position currentPosition() const;
		bool atEof() const;

		void emplaceToken(TokenKind kind);
    };
}