/*
 * Copyright (c) 2022-present, Treidex <treidexy@gmail.com>
 *
 * MIT License
 * 
 * As opposed to the Abstract Syntax Tree, this is an Abstract Checked Tree
 * ie: this tree has been checked through sema
 */

#pragma once

#include <llvm/IR/Instruction.h>

#include "sy/Ast.h"

namespace sy::act {
	enum class Kind {
		BinOp,
		Num,
	};

	struct Stmt {
		virtual Kind getKind() const = 0;
	};

	struct Type {
		enum Id {
			SInt,
			UInt,
			Float,
		};
	};

	struct Expr: Stmt {
		std::unique_ptr<Type> type;
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

		Kind getKind() const override {
			return Kind::BinOp;
		}
	};

	struct Num: Expr {
		NumConstant numConstant;

		Kind getKind() const override {
			return Kind::Num;
		}
	};
}