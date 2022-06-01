/*
 * Copyright (c) 2022-present, Treidex <treidexy@gmail.com>
 *
 * MIT License
 */

#pragma once

#include "sy/Air.h"

#include <llvm/IR/IRBuilder.h>

namespace sy::emit {
	using namespace llvm;

	class Emitter {
	private:
		air::Project project;
		LLVMContext &ctx;
		IRBuilder<> builder;

		std::vector<Type *> types;
		std::vector<Value *> vars;
		std::vector<Value *> funcs;
		std::unique_ptr<Module> module;

	public:
		Emitter(air::Project &&project, LLVMContext &ctx);

		std::unique_ptr<Module> emit();

	private:
		// we own the unique_ptrs, so this is 'safe'
		Value *emit(air::Stmt *node);

		Type *emit(air::TypeId node);

		Value *emit(air::VarInit *node);
		Function *emit(air::Func *node);

		Value *emit(air::Expr *node);
		Value *emit(air::BinOp *node);
		Value *emit(air::Num *node);

		Type *findType(air::TypeId node);
	};
} // sy
