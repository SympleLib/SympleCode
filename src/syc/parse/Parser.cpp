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
		uint64_t start = pos;
		AstNode *node = ParseNode();
		if (node)
			ast.push_back(node);
		while (peek().kind == TokenKind::Semicolon)
			next();

		if (start == pos)
			next();
	}

	return ast;
}

AstNode *Parser::ParseNode() {
	AstNode *node = ParseStmt();
	if (!node)
		node = ParseExpr();
	return node;
}

StmtAst *Parser::ParseStmt() {
	switch (peek().kind) {
	case TokenKind::Return:
		return ParseReturnStmt();
	default:
		return nullptr;
	}
}

ReturnStmtAst *Parser::ParseReturnStmt() {
	match(TokenKind::Return);
	if (peek().kind == TokenKind::Semicolon)
		return new ReturnStmtAst(nullptr);
	ExprAst *value = ParseExpr();
	return new ReturnStmtAst(value);
}

ExprAst *Parser::ParseExpr() {
	return ParseBinaryExpr(ParsePrimaryExpr());
}

ExprAst *Parser::ParseBinaryExpr(ExprAst *left) {
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

ExprAst *Parser::ParsePrimaryExpr() {
	Token tok = peek();
	switch (tok.kind) {
	case TokenKind::Integer:
		next();
		return new IntLiteralAst(std::stoll(tok.text));
	case TokenKind::Identifier:
		next();
		return new VariableExprAst(tok.text);

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
	case TokenKind::Equal:
		return BinaryOp::Assign;

	default:
		return BinaryOp::None;
	}
}

Token Parser::next() {
	if (pos < tokens.size())
		return tokens[pos++];

	return tokens.back();
}

Token Parser::match(TokenKind kind) {
	if (peek().kind == kind)
		return next();
	SourceLocation srcLoc = peek().sourceRange.start;
	printf("[%llu:%llu]: expected %s, found %s", srcLoc.line, srcLoc.column, peek().getName(), getTokenKindName(kind));
	return peek();
}

Token Parser::peek(int64_t offset) const {
	size_t idx = pos + offset;
	if (idx >= tokens.size())
		return tokens.back();
	return tokens[idx];
}