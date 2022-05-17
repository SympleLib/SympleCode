/*
 * Copyright (c) 2022-present, Treidex <treidexy@gmail.com>
 *
 * MIT License
 */

#include "Parser.h"

using namespace syc;

std::vector<AstNode *> Parser::Parse(const std::vector<Token> &tokens) {
	this->tokens = tokens;
	pos = 0;

	while (peek().kind != TokenKind::Eof) {
		ast.push_back(ParseExpr());
		while (peek().kind == TokenKind::Semicolon)
			next();
	}

	return ast;
}

ExprAst *Parser::ParseExpr() {
	return ParseBinExpr(ParseLiteral());
}

ExprAst *Parser::ParseBinExpr(ExprAst *left) {
	while (true) {
		BinaryOp op = getBinaryOperator(peek().kind);
		if (op == BinaryOp::None)
			break;

		next();

		ExprAst *right = ParseExpr();
		left = new BinaryExprAst(op, left, right);
	}

	return left;
}

LiteralAst *Parser::ParseLiteral() {
	Token tok = peek();
	switch (tok.kind) {
	case TokenKind::Integer:
		next();
		return new IntLiteralAst(std::stoll(tok.text));

	default:
		return nullptr;
	}
}

BinaryOp Parser::getBinaryOperator(TokenKind kind) {
	switch (kind) {
	case TokenKind::Plus:
		return BinaryOp::Add;
	case TokenKind::Minus:
		return BinaryOp::Sub;

	default:
		return BinaryOp::None;
	}
}

Token Parser::next() {
	if (pos < tokens.size())
		return tokens[pos++];

	return tokens.back();
}

Token Parser::peek(int64_t offset) const {
	size_t idx = pos + offset;
	if (idx >= tokens.size())
		return tokens.back();
	return tokens[idx];
}