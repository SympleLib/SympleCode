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

	while (!peek().is(TokenKind::Eof)) {
		uint64_t start = pos;
		AstNode *node = ParseNode();
		if (node)
			ast.push_back(node);

		// TODO: maybe be crazy??
		if (!(peek().flags & TokenFlag::StartOfLine) && !peek().is(TokenKind::Eof))
			match(TokenKind::Semicolon);

		while (peek().is(TokenKind::Semicolon))
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
	if (!peek().is(TokenKind::Identifier))
		return nullptr;

	return ParsePrimitiveType();
}

PrimitiveTypeAst *Parser::ParsePrimitiveType() {
	std::string name = next().text;
	uint64_t size = 32;
	TypeKind typeKind = scope->getType(name);
	if (peek().is(TokenKind::Colon)) {
		next();
		size = std::stoull(match(TokenKind::Integer).text);
	}

	return new PrimitiveTypeAst(TypeFlag::None, RefTypeKind::Value, typeKind, size);
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
	if (peek().is(TokenKind::Semicolon))
		return new ReturnStmtAst(nullptr);
	ExprAst *value = ParseExpr();
	return new ReturnStmtAst(value);
}

VariableStmtAst *Parser::ParseVariableStmt() {
	TypeAst *type = ParseType();
	std::string name = match(TokenKind::Identifier).text;
	ExprAst *init = nullptr;
	if (peek().is(TokenKind::Equal)) {
		next();
		init = ParseExpr();
	}

	VariableStmtAst *var = new VariableStmtAst(type, name, init);
	scope->defineVar(var);
	return var;
}


ExprAst *Parser::ParseExpr() {
	return ParseBinaryExpr(ParsePrefixExpr());
}

ExprAst *Parser::ParsePrefixExpr() {
	switch (peek().kind) {
	case TokenKind::Dollar:
		next();
		return new VariableExprAst(RefTypeKind::Value, scope->findVar(match(TokenKind::Identifier).text));
	case TokenKind::Identifier:
		return new VariableExprAst(RefTypeKind::Value, scope->findVar(match(TokenKind::Identifier).text));
	case TokenKind::Star:
		next();
		return new VariableExprAst(RefTypeKind::Weak, scope->findVar(match(TokenKind::Identifier).text));
	case TokenKind::Carot:
		next();
		return new VariableExprAst(RefTypeKind::Owned, scope->findVar(match(TokenKind::Identifier).text));
	case TokenKind::And:
		next();
		return new VariableExprAst(RefTypeKind::Temp, scope->findVar(match(TokenKind::Identifier).text));
	case TokenKind::Percent:
		next();
		return new VariableExprAst(RefTypeKind::Shared, scope->findVar(match(TokenKind::Identifier).text));

	default:
		return ParsePrimaryExpr();
	}
}

ExprAst *Parser::ParseBinaryExpr(ExprAst *left) {
	BinaryOp op = getBinaryOperator(peek().kind);
	if (op == BinaryOp::None)
		return left;

	next();

	ExprAst *right = ParseBinaryExpr(ParsePrefixExpr());
	return new BinaryExprAst(op, left, right);
}

ExprAst *Parser::ParsePrimaryExpr() {
	Token tok = peek();
	switch (tok.kind) {
	case TokenKind::Integer:
		next();
		return new IntLiteralAst(std::stoll(tok.text));
	case TokenKind::Identifier:
		next();
		std::cout << tok.text << ": " << scope->vars.size() << '\n';
		return new VariableExprAst(RefTypeKind::Value, scope->findVar(tok.text));

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
	if (peek().is(kind))
		return next();
	SourceLocation srcLoc = peek().sourceRange.start;
	std::cerr << SourceFile::getFilename(srcLoc.fileId) << "(" << srcLoc.line << "," << srcLoc.column << "): expected " << getTokenKindName(kind) << ", got " << peek().getName() << "\n";
	return next();
}

Token Parser::peek(int64_t offset) const {
	size_t idx = pos + offset;
	if (idx >= tokens.size())
		return tokens.back();
	return tokens[idx];
}