/*
 * Copyright (c) 2022-present, Treidex <treidexy@gmail.com>
 *
 * MIT License
 */

#include "Lexer.h"

#include <string>

using namespace syc;

const std::unordered_map<std::string_view, TokenKind> syc::tokenKeywords = {
#define KEYWORD(x, y) { #y, TokenKind::x },
#include "syc/parse/TokenKind.h"
};

std::vector<Token> Lexer::lex(SourceFileId srcFileId) {
	sourceFileId = srcFileId;
	source = SourceFile::getContent(sourceFileId);
	tokens.clear();

	line = 1;
	column = 1;

	pos = 0;

	while (!atEof()) {
		lexToken();
	}

	flags = TokenFlag::None;
	start = currentLocation();
	startPos = pos;
	next();
	emplaceToken(TokenKind::Eof);

	return tokens;
}

void Lexer::lexToken() {
	flags = TokenFlag::None;
	if (tokens.empty())
		flags = TokenFlag::StartOfLine;
	skipWhitespace();

	// comments
	if (peek() == '/' && peek(1) == '/') {
		while (peek() != '\n' && peek() != '\0') {
			next();
		}
	}

	skipWhitespace();

	start = currentLocation();
	startPos = pos;

	char c = peek();
	switch (c) {
	case '=':
		next();
		emplaceToken(TokenKind::Equal);
		break;
	case ';':
		next();
		emplaceToken(TokenKind::Semicolon);
		break;
	case '+':
		next();
		emplaceToken(TokenKind::Plus);
		break;
	case '-':
		next();
		emplaceToken(TokenKind::Minus);
		break;
	case ':':
		next();
		emplaceToken(TokenKind::Colon);
		break;
	case '$':
		next();
		emplaceToken(TokenKind::Dollar);
		break;
	case '*':
		next();
		emplaceToken(TokenKind::Star);
		break;
	case '^':
		next();
		emplaceToken(TokenKind::Carot);
		break;
	case '&':
		next();
		emplaceToken(TokenKind::And);
		break;
	case '%':
		next();
		emplaceToken(TokenKind::Percent);
		break;

	case '\0':
		break;

	default:
		if (std::isdigit(peek())) {
			lexNumberLiteral();
			break;
		}

		if (std::isalpha(peek()) || peek() == '_' || peek() == '$') {
			lexIdentifierOrKeyword();
			break;
		}

		next();
		emplaceToken(TokenKind::Unknown);
		break;
	}
}

void Lexer::lexIdentifierOrKeyword() {
	next();
	while (std::isalnum(peek()))
		next();

	TokenKind kind = TokenKind::Identifier;
	std::string_view text = source.substr(startPos, pos - startPos);
	if (tokenKeywords.contains(text))
		kind = tokenKeywords.at(text);

	emplaceToken(kind);
}

void Lexer::lexNumberLiteral() {
	next();
	while (std::isdigit(peek()))
		next();

	emplaceToken(TokenKind::Integer);
}

void Lexer::skipWhitespace() {
	if (std::isspace(peek())) {
		next();

		flags |= TokenFlag::LeadingSpace;
	}

	while (std::isspace(peek()))
		next();
}

void Lexer::emplaceToken(TokenKind kind) {
	SourceRange srcRange = {
			.start = start,
			.end = currentLocation(),
	};

	std::string_view str = source.substr(startPos, pos - startPos);
	tokens.emplace_back(kind, std::move(str), srcRange, flags);
}

char Lexer::next() {
	if (atEof()) {
		return '\0';
	}

	if (peek() == '\n') {
		++line;
		column = 0;

		flags |= TokenFlag::StartOfLine;
	}

	// right?
	if (peek() == '\r')
		column = 0;

	++column;

	return source[pos++];
}

char Lexer::peek(int64_t offset) const {
	size_t idx = pos + offset;
	if (idx >= source.length())
		return '\0';
	return source[idx];
}

bool Lexer::atEof() const {
	return pos >= source.length();
}

SourceLocation Lexer::currentLocation() const {
	return {
		.fileId = sourceFileId,
		.line = line,
		.column = column,
	};
}