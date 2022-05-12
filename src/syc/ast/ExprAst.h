/*
 * Copyright (c) 2022-present, Treidex <treidexy@gmail.com>
 *
 * MIT License
 */

#pragma once

#include "syc/Token.h"
#include "syc/ast/AstNode.h"

namespace syc {
	class ExprAst: public AstNode {
	public:
	};

	enum class BinaryOperator: uint8_t {
		None,

		Add,
		Sub,
	};

	class BinaryExprAst: public ExprAst {
	public:
		BinaryOperator op;
		ExprAst *left, *right;

	public:
		BinaryExprAst(BinaryOperator op, ExprAst *left, ExprAst *right):
			op(op), left(left), right(right) {
			kind = AstKind::BinaryExpr;
		}
	};
} // syc
