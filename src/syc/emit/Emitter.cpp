/*
 * Copyright (c) 2022-present, Treidex <treidexy@gmail.com>
 *
 * MIT License
 */

#include "Emitter.h"

using namespace syc;
using namespace syc::emit;

Emitter::Emitter(LLVMContext &ctx): ctx(ctx), builder(ctx) {}

Module *Emitter::Emit(const std::vector<AstNode *> &ast, StringRef moduleName) {
	module = new Module(moduleName, ctx);

	FunctionType *mainFuncType = FunctionType::get(builder.getInt32Ty(), ArrayRef<Type *> { builder.getInt32Ty(), builder.getInt8PtrTy(), }, false);
	mainFunc = Function::Create(mainFuncType, GlobalValue::ExternalLinkage, 0, "Main", module);

	BasicBlock *entry = BasicBlock::Create(ctx, "entry", mainFunc);
	builder.SetInsertPoint(entry);

	for (AstNode *node: ast)
		Emit(node);

	return module;
}

void Emitter::Emit(AstNode *node) {
	Value *expr = Emit((ExprAst *) node);
	Value *ptr = builder.CreateAlloca(builder.getInt32Ty());
	builder.CreateStore(expr, ptr, true);
}

Value *Emitter::Emit(ExprAst *node) {
	switch (node->getKind()) {
	case AstKind::BinaryExpr:
		return Emit((BinaryExprAst *) node);
	case AstKind::IntLiteral:
		return Emit((LiteralAst *) node);
	default:
		return nullptr;
	}
}

Value *Emitter::Emit(BinaryExprAst *node) {
	Value *left, *right;
	left = Emit(node->left);
	right = Emit(node->right);

	switch (node->op) {
	case BinaryOp::Add:
		return builder.CreateAdd(left, right);
	case BinaryOp::Sub:
		return builder.CreateSub(left, right);
	default:
		return nullptr;
	}
}


Value *Emitter::Emit(LiteralAst *node) {
	AstKind kind = node->getKind();

	if (kind == AstKind::IntLiteral) {
		IntLiteralAst *intNode = (IntLiteralAst *) node;
		return builder.getInt32(intNode->value);
	}

	return nullptr;
}