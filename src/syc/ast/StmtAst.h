/*
 * Copyright (c) 2022-present, Treidex <treidexy@gmail.com>
 *
 * MIT License
 */

#pragma once

#include "syc/parse/Token.h"
#include "syc/ast/AstNode.h"

namespace syc {
	class ExprAst;

	class StmtAst: public AstNode {
	};

	class ReturnStmtAst final: public StmtAst {
	public:
		ExprAst *value;

	public:
		ReturnStmtAst(ExprAst *value):
			value(value) {}

		AstKind getKind() const override {
			return AstKind::ReturnStmt;
		}

		void print(std::ostream &os, std::string indent = "", std::string_view label = "", bool last = true) const override;
	};
} // syc
