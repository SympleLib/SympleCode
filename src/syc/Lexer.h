/*
 * Copyright (c) 2022-present, Treidex <treidexy@gmail.com>
 *
 * MIT License
 */

#pragma once

#include "syc/Token.h"
#include <string_view>
#include <vector>

namespace syc {
    class Lexer {
	private:
		std::vector<Token> tokens;

		size_t pos;
		size_t line, column;

		SourceFileId sourceFileId;
		std::string_view source;

		size_t startPos;
		SourceLocation start;
		TokenFlags flags;

	public:
		Lexer() = default;
		std::vector<Token> lex(SourceFileId sourceFileId);

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

		SourceLocation currentLocation() const;
		bool atEof() const;

		void emplaceToken(TokenKind kind);
    };
}