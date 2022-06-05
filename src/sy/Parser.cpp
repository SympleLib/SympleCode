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

ast::Module Parser::parseModule() {
	ast::Module module;
	bool opened = false;

	while (!atEof()) {
		Token &tok = tokens[idx];
		switch (tok.kind) {
		case Token::RBrace:
			assert(opened && "unexpected `}`");
			goto Return;
			
		default: {
			std::unique_ptr<ast::Stmt> stmt = parseVarOrFunc();
			if (stmt->getKind() == ast::Kind::Var) {
				std::unique_ptr<ast::Var> var = cast<ast::Var, ast::Stmt>(std::move(stmt));
				module.vars.emplace_back(std::move(var));
			} else if (stmt->getKind() == ast::Kind::Func) {
				std::unique_ptr<ast::Func> func = cast<ast::Func>(std::move(stmt));
				module.funcs.emplace_back(std::move(func));
			} else {
				assert(false && "unexpected statement");
			}
		}
		}
	}

Return:
	return std::move(module);
}

std::unique_ptr<ast::Stmt> Parser::parseStmt() {
	if (tokens[idx].kind == Token::Identifier) {
		return parseVarOrFunc();
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

std::unique_ptr<ast::Stmt> Parser::parseVarOrFunc() {
	bool isPub = false;
	Token &maybeQual = tokens[idx];
	if (maybeQual.kind == Token::Identifier && maybeQual.text == "pub") {
		isPub = true;
		idx++;
	}

	std::unique_ptr<ast::Type> type = parseType();
	Token &nameTok = tokens[idx++];
	if (nameTok.kind != Token::Identifier) {
		abort();
	}

	if (tokens[idx].kind == Token::LParen) {
		std::unique_ptr<ast::Func> func = parseFunc(std::move(type), nameTok);
		if (isPub) {
			func->visibility = ast::Visibility::Public;
		}

		return func;
	} else {
		std::unique_ptr<ast::Var> var = parseVar(std::move(type), nameTok);
		if (isPub) {
			var->visibility = ast::Visibility::Public;
		}

		return var;
	}
}

std::unique_ptr<ast::Var> Parser::parseVar(std::unique_ptr<ast::Type> type, Token &name) {
	std::unique_ptr<ast::Var> var = std::make_unique<ast::Var>();
	var->type = std::move(type);
	var->name = name.text;

	if (tokens[idx].kind == Token::Equal) {
		idx++;
		var->init = std::move(parseExpr());
	}

	var->span = Span(fileId, var->type->span.start, var->init ? var->init->span.end : name.span.end);
	return std::move(var);
}

std::unique_ptr<ast::Func> Parser::parseFunc(std::unique_ptr<ast::Type> type, Token &name) {
	std::unique_ptr<ast::Func> func = std::make_unique<ast::Func>();
	func->type = std::move(type);
	func->name = name.text;

	if (tokens[idx++].kind != Token::LParen) {
		abort();
	}

	if (tokens[idx].kind != Token::RParen) {
		func->params.emplace_back(parseParam());
		while (tokens[idx].kind == Token::Comma) {
			idx++;
			func->params.emplace_back(parseParam());
		}
	}

	if (tokens[idx++].kind != Token::RParen) {
		abort();
	}

	if (tokens[idx++].kind != Token::LBrace) {
		abort();
	}

	while (tokens[idx].kind != Token::RBrace) {
		func->stmts.push_back(std::move(parseStmt()));
	}

	Token &rbraceTok = tokens[idx++];
	if (rbraceTok.kind != Token::RBrace) {
		abort();
	}

	func->span = Span(fileId, func->type->span.start, rbraceTok.span.end);
	return std::move(func);
}

ast::Param Parser::parseParam() {
	ast::Param param;
	param.type = parseType();

	Token &usageTok = tokens[idx];
	param.usage = Token::Percent;
	switch (usageTok.kind) {
	case Token::Carot:
	case Token::Ampersand:
	case Token::Percent:
		param.usage = usageTok.kind;
		idx++;
		break;
	}

	Token &nameTok = tokens[idx++];
	if (nameTok.kind != Token::Identifier) {
		abort();
	}
	param.name = nameTok.text;

	if (tokens[idx].kind == Token::Equal) {
		idx++;
		param.init = parseExpr();
	}

	param.span = Span(fileId, param.type->span.start, param.init ? param.init->span.end : nameTok.span.end);
	return std::move(param);
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