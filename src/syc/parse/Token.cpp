/*
 * Copyright (c) 2022-present, Treidex <treidexy@gmail.com>
 *
 * MIT License
 */

#include "Token.h"

using namespace syc;

Token::Token(TokenKind kind, std::string &&text, SourceRange sourceRange, TokenFlags flags): kind(kind), text(std::move(text)), sourceRange(sourceRange), flags(flags) {}
Token::Token(TokenKind kind, const std::string &text, SourceRange sourceRange, TokenFlags flags): kind(kind), text(text), sourceRange(sourceRange), flags(flags) {}
Token::Token(TokenKind kind, std::string_view text, SourceRange sourceRange, TokenFlags flags): kind(kind), text(text), sourceRange(sourceRange), flags(flags) {}

std::string Token::getFlagsAsString() const {
	std::string str;
	if (flags & TokenFlag::StartOfLine)
		str += "StartOfLine, ";
	if (flags & TokenFlag::LeadingSpace)
		str += "LeadingSpace, ";
	if (str.size() > 0)
		str.resize(str.size() - 2);
	return str;
}