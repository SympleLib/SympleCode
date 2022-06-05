/*
 * Copyright (c) 2022-present, Treidex <treidexy@gmail.com>
 *
 * MIT License
 */

#pragma once

#include "sy/Token.h"

namespace sy::ast {
	enum class Kind {
		Unknown,

		Var,
		Func,

		BinOp,
		Num,
	};

	enum class Visibility {
		Public,
		Private,
	};

	struct Stmt;
	struct Type;
	struct Expr;
	struct Module;

	struct Stmt {
		Span span = {};

		virtual Kind getKind() const {
			return Kind::Unknown;
		}
	};

	struct Type {
		Span span = {};

		enum Kind {
			Name,
			// Array,
		};

		virtual Kind getKind() const = 0;
	};

	struct NameType: Type {
		std::string name;

		Kind getKind() const override {
			return Kind::Name;
		}
	};

	struct Var: Stmt {
		Visibility visibility = Visibility::Private;
		std::unique_ptr<Type> type;
		std::string name;
		std::unique_ptr<Expr> init;

		Kind getKind() const override {
			return Kind::Var;
		}
	};

	struct Param {
		Span span = {};

		std::unique_ptr<Type> type = nullptr;
		Token::Kind usage = Token::Percent;
		std::string name = {};
		std::unique_ptr<Expr> init = nullptr;
	};

	struct Func: Stmt {
		Visibility visibility = Visibility::Private;
		std::unique_ptr<Type> type;
		std::string name;
		std::vector<Param> params;
		std::vector<std::unique_ptr<Stmt>> stmts;

		virtual Kind getKind() const {
			return Kind::Func;
		}
	};

	struct Expr: Stmt {
	};

	struct BinOp: Expr {
		Token::Kind op = Token::Kind::Unknown;
		std::unique_ptr<Expr> left = nullptr;
		std::unique_ptr<Expr> right = nullptr;

		Kind getKind() const override {
			return Kind::BinOp;
		}
	};

	struct Num: Expr {
		NumConstant numConstant = {};

		Kind getKind() const override {
			return Kind::Num;
		}
	};

	struct Module {
		Span span;
		std::string name;
		std::vector<std::unique_ptr<Var>> vars;
		std::vector<std::unique_ptr<Func>> funcs;

		std::vector<Module> subModules;
	};
}