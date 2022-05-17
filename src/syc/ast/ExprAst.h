/*
 * Copyright (c) 2022-present, Treidex <treidexy@gmail.com>
 *
 * MIT License
 */

#pragma once

#include "syc/parse/Token.h"
#include "syc/ast/AstNode.h"

namespace syc {
	class ExprAst: public AstNode {
	};

	enum class BinaryOp: uint8_t {
		None,

#define BINOP(x, y) x,
#include "syc/ast/Operator.h"

		Count,
	};

	constexpr const char *BinaryOpNames[(uint8_t) BinaryOp::Count] {
		"None",

#define BINOP(x, y) y,
#include "syc/ast/Operator.h"
	};

	ENUM_NAME_FUNC(BinaryOp);

	class BinaryExprAst final: public ExprAst {
	public:
		BinaryOp op;
		ExprAst *left, *right;

	public:
		BinaryExprAst(BinaryOp op, ExprAst *left, ExprAst *right):
			op(op), left(left), right(right) {}

		AstKind getKind() const override {
			return AstKind::BinaryExpr;
		}

		void print(std::ostream &os, std::string indent = "", std::string_view label = "", bool last = true) const override;
	};
} // syc
