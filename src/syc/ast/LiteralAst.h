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
		bool isMutable() const final override {
			return false;
		}

		RefTypeKind getRefKind() const final override {
			return RefTypeKind::Value;
		}
	};

	class IntLiteralAst: public LiteralAst {
	public:
		int64_t value;

	public:
		IntLiteralAst(int64_t value):
			value(value) {}

		AstKind getKind() const override {
			return AstKind::IntLiteral;
		}

		void print(std::ostream &os, std::string indent = "", std::string_view label = "", bool last = true) const override;
	};
} // syc
