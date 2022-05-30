/*
 * Copyright (c) 2022-present, Treidex <treidexy@gmail.com>
 *
 * MIT License
 * 
 * As opposed to the Abstract Syntax Tree, this is an Abstract Intermediate Representation
 * ie: this tree has been checked through sema
 */

#pragma once

#include <llvm/IR/Instruction.h>

#include "sy/Ast.h"

namespace sy::air {
	using TypeId = uint64_t;
	using ScopeId = uint64_t;

	enum class Kind {
		Func,

		BinOp,
		Num,
	};

	struct Stmt;
	struct Expr;

	struct Stmt {
		virtual Kind getKind() const = 0;
	};

	struct Func: Stmt {
		TypeId typeId;
		std::string name;
		std::vector<std::unique_ptr<Stmt>> stmts;

		Kind getKind() const override {
			return Kind::Func;
		}
	};

	struct Expr: Stmt {
		TypeId typeId;
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

	struct Type {
		enum Kind {
			SInt,
			UInt,
			Float,
		} kind;
	};

	struct Scope {
		Scope *parent;
	};

	struct Project {
		std::vector<Type> types = { Type { Type::SInt }, Type { Type::UInt }, Type { Type::Float } };
		std::vector<Scope> scopes;
		std::vector<std::unique_ptr<Stmt>> stmts;

		TypeId findOrAddTypeId(Type &&type) {
			for (size_t i = 0; i < types.size(); i++) {
				if (types[i].kind == type.kind) {
					return i;
				}
			}

			types.emplace_back(std::move(type));
			return types.size() - 1;
		}

		ScopeId createScope(ScopeId parent) {
			Scope scope;
			scope.parent = &scopes[parent];
			scopes.push_back(scope);
			return scopes.size() - 1;
		}
	};
}