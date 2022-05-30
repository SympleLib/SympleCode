/*
 * Copyright (c) 2022-present, Treidex <treidexy@gmail.com>
 *
 * MIT License
 */

#include "sy/Lexer.h"

using namespace sy;

std::vector<Token> sy::lex(FileId fileId, std::string_view src) {
	std::vector<Token> out;
	uint64_t idx = 0;

	while (idx < src.length()) {
		char c = src[idx];

		uint64_t start = idx;

		if (c == '+') {
			out.emplace_back(Token::Plus, Span(fileId, start, ++idx));
		} else if (c == '-') {
			out.emplace_back(Token::Minus, Span(fileId, start, ++idx));
		} else if (c == '*') {
			out.emplace_back(Token::Star, Span(fileId, start, ++idx));
		} else if (c == '/') {
			out.emplace_back(Token::Slash, Span(fileId, start, ++idx));
		} else if (c == '(') {
			out.emplace_back(Token::LParen, Span(fileId, start, ++idx));
		} else if (c == ')') {
			out.emplace_back(Token::RParen, Span(fileId, start, ++idx));
		} else if (c == ',') {
			out.emplace_back(Token::Comma, Span(fileId, start, ++idx));
		} else if (c == '=') {
			out.emplace_back(Token::Equal, Span(fileId, start, ++idx));
		} else if (c == '{') {
			out.emplace_back(Token::LBrace, Span(fileId, start, ++idx));
		} else if (c == '}') {
			out.emplace_back(Token::RBrace, Span(fileId, start, ++idx));
		} else if (std::isdigit(c)) {
			while (idx < src.length() && std::isdigit(src[idx])) {
				idx++;
			}

			out.emplace_back(Token::Number, NumConstant { .unsignedVal = std::stoull(std::string(src.substr(start, idx - start))), .kind = NumConstant::UInt }, Span(fileId, start, idx));
		} else if (std::isalpha(c)) {
			while (idx < src.length() && std::isalnum(src[idx])) {
				idx++;
			}

			out.emplace_back(Token::Identifier, src.substr(start, idx - start), Span(fileId, start, idx));
		} else if (std::isspace(c)) {
			idx++;
		} else {
			idx++;
			out.emplace_back(Token::Unknown, Span(fileId, start, idx));
		}
	}

	out.emplace_back(Token::Eof, Span(fileId, idx, idx + 1));

	return std::move(out);
}