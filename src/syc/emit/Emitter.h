/*
 * Copyright (c) 2022-present, Treidex <treidexy@gmail.com>
 *
 * MIT License
 */

#pragma once

#include "syc/ast/Ast.h"

#include <llvm/IR/IRBuilder.h>

namespace syc {
	namespace emit {
		using namespace llvm;

		class Emitter {
		public:
			LLVMContext &ctx;
			Module *module;
			Function *mainFunc;
			IRBuilder<> builder;

			std::unordered_map<std::string_view, Value *> vars;

		public:
			Emitter(LLVMContext &ctx);

			Module *Emit(const std::vector<AstNode *> &ast, StringRef moduleName);

		private:
			// should be const *, but I'm too lazy
			void Emit(AstNode *);

			void Emit(StmtAst *);
			void Emit(ReturnStmtAst *);

			Value *Emit(ExprAst *);
			Value *Emit(BinaryExprAst *);
			Value *Emit(VariableExprAst *);

			Value *EmitRef(ExprAst *);
			Value *EmitRef(VariableExprAst *);

			Value *Emit(LiteralAst *);
		};
	}
} // syc
