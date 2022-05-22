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
	class TypeAst;

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

	class VariableStmtAst final: public StmtAst {
	public:
		TypeAst *type;
		std::string name;
		ExprAst *init;

	public:
		VariableStmtAst(TypeAst *type, std::string_view name, ExprAst *init):
				type(type), name(name), init(init) {}

		AstKind getKind() const override {
			return AstKind::VariableStmt;
		}

		void print(std::ostream &os, std::string indent = "", std::string_view label = "", bool last = true) const override;
	};
} // syc
