/*
 * Copyright (c) 2022-present, Treidex <treidexy@gmail.com>
 *
 * MIT License
 */

#include "sy/Sema.h"

using namespace sy;

Sema::Sema(std::vector<std::unique_ptr<ast::Stmt>> &&parsedStmts)
	: parsedStmts(std::move(parsedStmts)) {}

air::Project Sema::check() {
	for (std::unique_ptr<ast::Stmt> &parsedStmt : parsedStmts) {
		project.stmts.emplace_back(std::move(checkStmt(std::move(parsedStmt))));
	}

	return std::move(project);
}

std::unique_ptr<air::Stmt> Sema::checkStmt(std::unique_ptr<ast::Stmt> parsed) {
	return checkExpr(std::move(cast<ast::Expr>(std::move(parsed))));
}

std::unique_ptr<air::Expr> Sema::checkExpr(std::unique_ptr<ast::Expr> parsed) {
	switch (parsed->getKind()) {
	case ast::Kind::BinOp:
		return checkBinOp(std::move(cast<ast::BinOp>(std::move(parsed))));
	case ast::Kind::Num:
		return checkNum(std::move(cast<ast::Num>(std::move(parsed))));

	default:
		abort();
	}
}


std::unique_ptr<air::BinOp> Sema::checkBinOp(std::unique_ptr<ast::BinOp> parsed) {
	std::unique_ptr<air::Expr> left = checkExpr(std::move(parsed->left));
	std::unique_ptr<air::Expr> right = checkExpr(std::move(parsed->right));
	air::TypeId typeId = left->typeId;
	assert(left->typeId == right->typeId && "BinOp: left and right types must be the same");

	air::BinOp::OpKind opKind;
	switch (parsed->op) {
	case Token::Kind::Plus:
		opKind = air::BinOp::OpKind::Add;
		break;
	case Token::Kind::Minus:
		opKind = air::BinOp::OpKind::Sub;
		break;
	case Token::Kind::Star:
		opKind = air::BinOp::OpKind::Mul;
		break;
	case Token::Kind::Slash:
		opKind = air::BinOp::OpKind::Div;
		break;
	}

	std::unique_ptr<air::BinOp> expr = std::make_unique<air::BinOp>();
	expr->opKind = opKind;
	expr->typeId = typeId;
	expr->left = std::move(left);
	expr->right = std::move(right);
	return std::move(expr);
}

std::unique_ptr<air::Num> Sema::checkNum(std::unique_ptr<ast::Num> parsed) {
	std::unique_ptr<air::Num> expr = std::make_unique<air::Num>();
	expr->numConstant = parsed->numConstant;
	air::Type type;
	switch (parsed->numConstant.kind) {
	case NumConstant::SInt:
		type.kind = air::Type::SInt;
		break;
	case NumConstant::UInt:
		type.kind = air::Type::UInt;
		break;
	case NumConstant::Float:
		type.kind = air::Type::Float;
		break;
	}

	expr->typeId = project.findOrAddTypeId(std::move(type));

	return std::move(expr);
}