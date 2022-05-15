/*
 * Copyright (c) 2022-present, Treidex <treidexy@gmail.com>
 *
 * MIT License
 */

#include "syc/emit/Emitter.h"

using namespace syc;
using namespace llvm;

Emitter::Emitter(LLVMContext &context): ctx(context), builder(context) {}

llvm::Module *Emitter::Emit(StringRef moduleId, std::vector<AstNode *> _ast) {
	ast = _ast;

	module = new llvm::Module(moduleId, ctx);

	FunctionType *fnTy = FunctionType::get(Type::getVoidTy(ctx), {}, false);
	Function *func = Function::Create(fnTy, Function::PrivateLinkage, ":)", *module);

	BasicBlock *entry = BasicBlock::Create(ctx, "entry", func);


	for (AstNode *node: ast)
		Emit(node);

	return module;
}

void Emitter::Emit(AstNode *) {

}