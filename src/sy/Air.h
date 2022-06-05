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
	using GlobalId = uint64_t;
	using FuncId = uint64_t;
	using ScopeId = uint64_t;
	struct VarId {
		ScopeId scopeId;
		uint64_t id;

		bool operator==(const VarId &other) const {
			return scopeId == other.scopeId && id == other.id;
		}
	};

	enum class Kind {
		Var,
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
		VarId id;
		TypeId typeId;
		std::string name;
		std::unique_ptr<Expr> init;

		Kind getKind() const override {
			return Kind::Var;
		}
	};
	
	struct Param: Var {
		enum Usage {
			Move,
			Borrow,
			Ref,
		} usage = Ref;

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
		ScopeId id;

		std::vector<std::unique_ptr<Var>> vars = {};
		std::unordered_map<std::string_view, FuncId> funcs = {};

		VarId addVar(std::unique_ptr<Var> var) {
			var->id = { id, vars.size() };
			vars.push_back(std::move(var));
			return vars.back()->id;
		}
	};

	struct Project {
		std::vector<Type> types = { Type { Type::SInt }, Type { Type::UInt }, Type { Type::Float } };
		std::vector<std::unique_ptr<Func>> funcs;
		std::vector<Scope> scopes = { Scope { 0, 0, } };

		TypeId findOrAddTypeId(Type &&type) {
			for (size_t i = 0; i < types.size(); i++) {
				if (types[i].kind == type.kind) {
					return i;
				}
			}

			types.emplace_back(std::move(type));
			return types.size() - 1;
		}

		FuncId addFunc(std::unique_ptr<Func> func) {
			funcs.emplace_back(std::move(func));
			return funcs.size() - 1;
		}

		ScopeId createScope(ScopeId parent) {
			Scope scope;
			scope.parent = parent;
			scope.id = scopes.size();
			scopes.push_back(scope);
			return scope.id;
		}
	};
}


namespace std {
	template <>
	struct hash<sy::air::VarId> {
		size_t operator()(const sy::air::VarId &var) const {
			return hash<sy::air::ScopeId>()(var.scopeId) * 31 + hash<uint64_t>()(var.id);
		}
	};
}