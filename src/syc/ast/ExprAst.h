/*
 * Copyright (c) 2022-present, Treidex <treidexy@gmail.com>
 *
 * MIT License
 */

#pragma once

#include "syc/parse/Token.h"
#include "syc/ast/AstNode.h"

namespace syc {
	class VariableStmtAst;
	class TypeAst;

	enum class ExprUsage: uint8_t {
#define EXPR_USAGE(x, y) x,
#include "syc/ast/ExprUsage.h"

		Count,
	};

	constexpr const char *ExprUsageNames[(uint8_t) ExprUsage::Count] {
#define EXPR_USAGE(x, y) #x " '" #y "'",
#include "syc/ast/ExprUsage.h"
	};

	ENUM_NAME_FUNC(ExprUsage);

	class ExprAst: public AstNode {
	public:
		// changed in parsing :)
		ExprUsage usage = ExprUsage::Copying;

	public:
		virtual bool isMutable() const = 0;

	protected:
		void printExprIndent(std::ostream &os, std::string_view indent, std::string_view label, bool last) const {
			printIndent(os, indent, label, last);
			os << getExprUsageName(usage) << ' ';
		}
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

		bool isMutable() const override {
			return false;
		}

		void print(std::ostream &os, std::string indent = "", std::string_view label = "", bool last = true) const override;
	};

	class VariableExprAst final: public ExprAst {
	public:
		VariableStmtAst *var;

	public:
		VariableExprAst(VariableStmtAst *var):
			var(var) {}

		AstKind getKind() const override {
			return AstKind::VariableExpr;
		}

		bool isMutable() const override {
			return true; // TODO: auto const
		}

		void print(std::ostream &os, std::string indent = "", std::string_view label = "", bool last = true) const override;
	};
} // syc
