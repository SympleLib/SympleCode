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
	if (tokens[idx].kind == Token::Identifier) {
		return parseFunc();
	} else {
		return parseExpr();
	}
}

std::unique_ptr<ast::Type> Parser::parseType() {
	// just parsing a name type at this point
	Token &tok = tokens[idx++];
	if (tok.kind != Token::Identifier) {
		abort();
	}

	std::unique_ptr<ast::NameType> nameType = std::make_unique<ast::NameType>();
	nameType->span = tok.span;
	nameType->name = tok.text;

	return std::move(nameType);
}

std::unique_ptr<ast::Func> Parser::parseFunc() {
	std::unique_ptr<ast::Type> type = parseType();
	Token &nameTok = tokens[idx++];
	if (nameTok.kind != Token::Identifier) {
		abort();
	}

	if (tokens[idx++].kind != Token::LParen) {
		abort();
	}

	// std::vector<ast::Param> params;
	// if (tokens[idx].kind != Token::RParen) {
	// 	params.push_back(parseParam());
	// 	while (tokens[idx].kind == Token::Comma) {
	// 		idx++;
	// 		params.push_back(parseParam());
	// 	}
	// }

	if (tokens[idx++].kind != Token::RParen) {
		abort();
	}

	if (tokens[idx++].kind != Token::LBrace) {
		abort();
	}

	std::vector<std::unique_ptr<ast::Stmt>> stmts;
	while (tokens[idx].kind != Token::RBrace) {
		stmts.push_back(std::move(parseStmt()));
	}

	if (tokens[idx++].kind != Token::RBrace) {
		abort();
	}

	std::unique_ptr<ast::Func> func = std::make_unique<ast::Func>();
	func->span = Span(fileId, type->span.start, tokens[idx - 1].span.end);
	func->type = std::move(type);
	func->name = nameTok.text;
	// func->params = std::move(params);
	func->stmts = std::move(stmts);

	return std::move(func);
}

// ast::Param Parser::parseParam() {
// 	std::unique_ptr<ast::Type> type = parseType();
// 	Token &nameTok = tokens[idx++];
// 	if (nameTok.kind != Token::Identifier) {
// 		abort();
// 	}

// 	std::unique_ptr<ast::Expr> init;
// 	if (tokens[idx].kind == Token::Equal) {
// 		idx++;
// 		init = parseExpr();
// 	}

// 	ast::Param param;
// 	param.span = nameTok.span;
// 	param.name = nameTok.text;
// 	param.type = std::move(type);
// 	param.init = std::move(init);

// 	return param;
// }

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
	case Token::Number:
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