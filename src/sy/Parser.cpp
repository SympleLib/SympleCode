/*
 * Copyright (c) 2022-present, Treidex <treidexy@gmail.com>
 *
 * MIT License
 */

#include "sy/Parser.h"

using namespace sy;

uint8_t Parser::getBinOpPrecedence(Token::Kind kind) {
	switch (kind) {
	case Token::Kind::Plus:
	case Token::Kind::Minus:
		return 1;

	case Token::Kind::Star:
	case Token::Kind::Slash:
		return 2;

	default:
		return 0;
	}
}

Parser::Parser(FileId fileId, std::vector<Token> &&tokens)
	: fileId(fileId), tokens(std::move(tokens)) {}

std::vector<std::unique_ptr<ast::Stmt>> Parser::parse() {
	while (!atEof()) {
		stmts.push_back(std::move(parseStmt()));
	}

	return std::move(stmts);
}

std::unique_ptr<ast::Stmt> Parser::parseStmt() {
	return parseExpr();
}

std::unique_ptr<ast::Type> parseType() {
	return nullptr;
}

std::unique_ptr<ast::Expr> Parser::parseExpr() {
	return parseBinOp();
}

std::unique_ptr<ast::Expr> Parser::parseBinOp(uint8_t precedence) {
	std::unique_ptr<ast::Expr> left = parsePrimary();

	while (!atEof()) {
		Token::Kind op = tokens[idx].kind;
		uint8_t opPrecedence = getBinOpPrecedence(op);

		if (opPrecedence <= precedence) {
			return std::move(left);
		}

		idx++;
		std::unique_ptr<ast::Expr> right = parseBinOp(opPrecedence);
		
		std::unique_ptr<ast::BinOp> expr = std::make_unique<ast::BinOp>();
		expr->span = Span(fileId, left->span.start, right->span.end);
		expr->left = std::move(left);
		expr->right = std::move(right);
		expr->op = op;

		left = std::move(expr);
	}

	return std::move(left);
}

std::unique_ptr<ast::Expr> Parser::parsePrimary() {
	switch (tokens[idx].kind) {
	case Token::Kind::Number:
		return parseNum();

	default:
		abort();
	}
}

std::unique_ptr<ast::Num> Parser::parseNum() {
	std::unique_ptr<ast::Num> expr = std::make_unique<ast::Num>();

	Token &token = tokens[idx++];
	expr->numConstant = token.numConstant;
	expr->span = token.span;
	return expr;
}

bool Parser::atEof() {
	return idx >= tokens.size() - 1;
}