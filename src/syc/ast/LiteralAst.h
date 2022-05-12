/*
 * Copyright (c) 2022-present, Treidex <treidexy@gmail.com>
 *
 * MIT License
 */

#pragma once

#include "syc/ast/ExprAst.h"

namespace syc {
	class LiteralAst: public ExprAst {
	public:
	};

	class IntLiteralAst: public LiteralAst {
	public:
		int64_t value;

	public:
		IntLiteralAst(int64_t value):
			value(value) {
			kind = AstKind::IntLiteral;
		}

		int Eval() override {
			return value;
		}
	};
} // syc
