/*
 * Copyright (c) 2022-present, Treidex <treidexy@gmail.com>
 *
 * MIT License
 */

#include "Token.h"

using namespace Symple;

Token::Token(TokenKind kind, std::string &&text, SourceRange sourceRange): kind(kind), text(std::move(text)), sourceRange(sourceRange) {}
Token::Token(TokenKind kind, const std::string &text, SourceRange sourceRange): kind(kind), text(text), sourceRange(sourceRange) {}
Token::Token(TokenKind kind, std::string_view text, SourceRange sourceRange): kind(kind), text(text), sourceRange(sourceRange) {}