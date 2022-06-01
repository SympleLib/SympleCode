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
	using VarId = struct { uint64_t idInScope; ScopeId scopeId; };
	using GlobalId = uint64_t;
	using FuncId = uint64_t;
	using ScopeId = uint64_t;

	enum class Kind {
		Var,
		VarInit,
		Param,
		Func,

		BinOp,
		Num,
		Null,
	};

	struct Stmt;
	struct Expr;

	struct Stmt {
		virtual Kind getKind() const = 0;
	};

	struct Var: Stmt {
		TypeId typeId;
		std::string name;

		Kind getKind() const override {
			return Kind::Var;
		}
	};

	struct VarInit: Stmt {
		VarId varId;
		std::unique_ptr<Expr> expr;

		Kind getKind() const override {
			return Kind::VarInit;
		}
	};
	
	struct Param: Var {
		enum Usage {
			Move,
			Borrow,
			Ref,
		} usage = Ref;
		std::unique_ptr<Expr> init;

		Kind getKind() const override {
			return Kind::Param;
		}
	};

	struct Func: Stmt {
		TypeId typeId;
		std::string name;
		std::vector<VarId> params;
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

	struct Null: Expr {
		Kind getKind() const override {
			return Kind::Null;
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
		ScopeId parent;

		std::vector<VarId> varIds;
		std::vector<FuncId> funcIds;
	};

	struct Project {
		std::vector<Type> types = { Type { Type::SInt }, Type { Type::UInt }, Type { Type::Float } };
		std::vector<Var> globals;
		std::vector<Func> funcs;
		std::vector<Scope> scopes = { Scope { 0 } };
		// no globals should be included in this list
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

		VarId addGlobal(Var &&global) {
			globals.emplace_back(std::move(global));
			return globals.size() - 1;
		}

		FuncId addFunc(Func &&func) {
			funcs.emplace_back(std::move(func));
			return funcs.size() - 1;
		}

		ScopeId createScope(ScopeId parent) {
			Scope scope;
			scope.parent = parent;
			scopes.push_back(scope);
			return scopes.size() - 1;
		}
	};
}