/*
 * Copyright (c) 2022-present, Treidex <treidexy@gmail.com>
 *
 * MIT License
 */

#include "syc/parse/Parser.h"

#include <iostream>

using namespace syc;

std::vector<AstNode *> Parser::Parse(const std::vector<Token> &tokens) {
	this->tokens = tokens;
	pos = 0;

	globalScope = ParseScope();
	globalScope.defineType("int", TypeKind::Int);
	globalScope.defineType("float", TypeKind::Float);
	scope = &globalScope;

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


TypeAst *Parser::ParseType() {
	if (peek().kind != TokenKind::Identifier)
		return nullptr;

	return ParsePrimitiveType();
}

PrimitiveTypeAst *Parser::ParsePrimitiveType() {
	std::string_view name = next().text;
	TypeKind typeKind = scope->getType(name);
	uint64_t size = std::stoull(match(TokenKind::Integer).text);
	return new PrimitiveTypeAst(false, typeKind, size);
}


StmtAst *Parser::ParseStmt() {
	Token tok = peek();
	switch (tok.kind) {
	case TokenKind::Return:
		return ParseReturnStmt();
	case TokenKind::Identifier:
		if (scope->hasType(tok.text))
			return ParseVariableStmt();
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

VariableStmtAst *Parser::ParseVariableStmt() {
	TypeAst *type = ParseType();
	std::string name = match(TokenKind::Identifier).text;
	ExprAst *init = nullptr;
	if (peek().kind == TokenKind::Equal) {
		next();
		init = ParseExpr();
	}

	VariableStmtAst *var = new VariableStmtAst(type, name, init);
	scope->defineVar(var);
	return var;
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
		return new VariableExprAst(scope->findVar(tok.text));

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
	return next();
}

Token Parser::peek(int64_t offset) const {
	size_t idx = pos + offset;
	if (idx >= tokens.size())
		return tokens.back();
	return tokens[idx];
}