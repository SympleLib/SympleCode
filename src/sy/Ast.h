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

		Stmt() = default;
		virtual Kind getKind() const = 0;
	};

	struct Type {
	};

	struct Expr: Stmt {
		Expr() = default;
	};

	struct BinOp: Expr {
		enum OpKind {
			Add,
			Sub,
			Mul,
			Div,
		} opKind = Add;
		std::unique_ptr<Expr> left = nullptr;
		std::unique_ptr<Expr> right = nullptr;

		BinOp() = default;
		Kind getKind() const override {
			return Kind::BinOp;
		}
	};

	struct Num: Expr {
		NumConstant numConstant = {};

		Num() = default;
		Kind getKind() const override {
			return Kind::Num;
		}
	};
}