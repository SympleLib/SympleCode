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

		Func,

		BinOp,
		Num,
	};

	struct Stmt;
	struct Type;
	struct Expr;

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

	struct Param {
		Span span = {};

		std::unique_ptr<Type> type = nullptr;
		std::string name = {};
		std::unique_ptr<Expr> init = nullptr;
	};

	struct Func: Stmt {
		std::unique_ptr<Type> type;
		std::string name;
		// std::vector<Param> params;
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
}