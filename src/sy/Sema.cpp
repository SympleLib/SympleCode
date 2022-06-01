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
		check(parsedStmt.get());
	}

	return std::move(project);
}

std::unique_ptr<air::Stmt> Sema::check(ast::Stmt *parsed) {
	switch (parsed->getKind()) {
	case ast::Kind::Var:
		return check((ast::Var *) parsed);
	case ast::Kind::Func:
		return check((ast::Func *) parsed);

	default:
		return check((ast::Expr *) parsed);
	}
}

air::TypeId Sema::check(ast::Type *parsed) {
	air::Type type;
	switch (parsed->getKind()) {
	case ast::Type::Name: {
		ast::NameType *parsedName = (ast::NameType *) parsed;
		if (parsedName->name == "i32") {
			type.kind = air::Type::SInt;
		} else if (parsedName->name == "u32") {
			type.kind = air::Type::UInt;
		} else if (parsedName->name == "f32") {
			type.kind = air::Type::Float;
		} else {
			abort();
		}
		break;
	}

	default:
		abort();
	}

	return project.findOrAddTypeId(std::move(type));
}

std::unique_ptr<air::VarInit> Sema::check(ast::Var *parsed) {
	std::unique_ptr<air::Var> var = std::make_unique<air::Var>();
	var->typeId = check(parsed->type.get());
	var->name = parsed->name;

	air::VarId varId = project.addVar(std::move(var));

	std::unique_ptr<air::VarInit> varInit = std::make_unique<air::VarInit>();
	varInit->varId = varId;
	varInit->expr = std::move(check(parsed->init.get()));
	
	return std::move(varInit);
}

std::unique_ptr<air::Func> Sema::check(ast::Func *parsed) {
	std::unique_ptr<air::Func> func = std::make_unique<air::Func>();
	func->typeId = check(parsed->type.get());
	func->name = parsed->name;

	for (ast::Param &param : parsed->params) {
		func->params.emplace_back(check(param));
	}

	for (std::unique_ptr<ast::Stmt> &parsedStmt : parsed->stmts) {
		func->stmts.emplace_back(check(parsedStmt.get()));
	}

	return std::move(func);
}

air::VarId Sema::check(ast::Param &parsed) {
	std::unique_ptr<air::Param> param = std::make_unique<air::Param>();
	param->typeId = check(parsed.type.get());
	param->name = parsed.name;
	param->init = check(parsed.init.get());

	return project.addVar(std::move(param));
}

std::unique_ptr<air::Expr> Sema::check(ast::Expr *parsed) {
	if (parsed == nullptr)
		return std::make_unique<air::Null>();

	switch (parsed->getKind()) {
	case ast::Kind::BinOp:
		return check((ast::BinOp *) parsed);
	case ast::Kind::Num:
		return check((ast::Num *) parsed);

	default:
		abort();
	}
}


std::unique_ptr<air::BinOp> Sema::check(ast::BinOp *parsed) {
	std::unique_ptr<air::Expr> left = check(parsed->left.get());
	std::unique_ptr<air::Expr> right = check(parsed->right.get());
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

std::unique_ptr<air::Num> Sema::check(ast::Num *parsed) {
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