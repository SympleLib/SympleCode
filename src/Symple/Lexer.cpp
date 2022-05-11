/*
 * Copyright (c) 2022-present, Treidex <treidexy@gmail.com>
 *
 * MIT License
 */

#include "Lexer.h"

#include <string>

using namespace Symple;

std::vector<Token> Lexer::lex(std::string_view src) {
	source = src;
	tokens.clear();

	line = 1;
	column = 1;

	while (!atEof()) {
		lexToken();
	}

	return tokens;
}

void Lexer::lexToken() {
	skipWhitespace();

	start = currentPosition();
	startPos = pos;

	if (atEof()) {
		next();
		emplaceToken(TokenKind::Unknown);
	}
	else if (std::isdigit(peek()))
		lexNumberLiteral();
	else if (std::isalpha(peek()) || peek() == '_' || peek() == '$')
		lexIdentifierOrKeyword();
	else {
		next();
		emplaceToken(TokenKind::Unknown);
	}
}

void Lexer::lexIdentifierOrKeyword() {
	next();
	while (std::isalnum(peek()))
		next();

	emplaceToken(TokenKind::Identifier);
}

void Lexer::lexNumberLiteral() {
	next();
	while (std::isdigit(peek()))
		next();

	emplaceToken(TokenKind::Integer);
}

void Lexer::skipWhitespace() {
	while (std::isspace(peek()))
		next();
}

void Lexer::emplaceToken(TokenKind kind) {
	SourceRange srcRange = {
			.start = start,
			.end = currentPosition(),
	};

	std::string_view str = source.substr(startPos, pos - startPos);
	tokens.emplace_back(kind, std::move(str), srcRange);
}

char Lexer::next() {
	if (atEof()) {
		return '\0';
	}

	if (peek() == '\n') {
		++line;
		column = 0;
	}

	++column;
	++pos;
}

char Lexer::peek(ssize_t offset) const {
	size_t idx = pos + offset;
	if (idx >= source.length())
		return '\0';
	return source[idx];
}

bool Lexer::atEof() const {
	return pos >= source.length();
}

Position Lexer::currentPosition() const {
	return {
			.line = line,
			.column = column,
	};
}