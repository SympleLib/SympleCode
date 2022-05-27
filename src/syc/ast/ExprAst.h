/*
 * Copyright (c) 2022-present, Treidex <treidexy@gmail.com>
 *
 * MIT License
 */

#pragma once

#include "syc/parse/Token.h"
#include "syc/ast/AstNode.h"
#include "syc/ast/RefTypeKind.h"

namespace syc {
	class VariableStmtAst;
	class TypeAst;

	class ExprAst: public AstNode {
	public:
		virtual bool isMutable() const = 0;
		virtual RefTypeKind getRefKind() const = 0;

	protected:
		void printExprIndent(std::ostream &os, std::string_view indent, std::string_view label, bool last) const {
			printIndent(os, indent, label, last);
			os << getRefTypeKindName(getRefKind());
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

#define BINOP(x, y) #x " '" y "'",
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

		RefTypeKind getRefKind() const override {
			return RefTypeKind::Value;
		}

		void print(std::ostream &os, std::string indent = "", std::string_view label = "", bool last = true) const override;
	};

	class VariableExprAst final: public ExprAst {
	public:
		RefTypeKind refKind;
		VariableStmtAst *var;

	public:
		VariableExprAst(RefTypeKind refKind, VariableStmtAst *var):
			refKind(refKind), var(var) {}

		AstKind getKind() const override {
			return AstKind::VariableExpr;
		}

		bool isMutable() const override {
			return true; // TODO: auto const
		}

		RefTypeKind getRefKind() const override {
			return refKind;
		}

		void print(std::ostream &os, std::string indent = "", std::string_view label = "", bool last = true) const override;
	};
} // syc
