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
		BinOp,
		Num,
	};

	struct Stmt {
		Span span = {};

		virtual Kind getKind() const {
			return Kind::Unknown;
		}
	};

	struct Type {
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