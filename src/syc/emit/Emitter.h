/*
 * Copyright (c) 2022-present, Treidex <treidexy@gmail.com>
 *
 * MIT License
 */

#pragma once

#include "syc/ast/Ast.h"

#include <llvm/IR/IRBuilder.h>

namespace syc {
	class Emitter {
	private:
		llvm::LLVMContext &ctx;
		llvm::Module *module;
		llvm::IRBuilder<> builder;

		std::vector<AstNode *> ast;

	public:
		Emitter(llvm::LLVMContext &context);

		llvm::Module *Emit(llvm::StringRef moduleId, std::vector<AstNode *>);

	private:
		void Emit(AstNode *);
	};
} // syc
