/*
 * Copyright (c) 2022-present, Treidex <treidexy@gmail.com>
 *
 * MIT License
 */

#include "sy/Emitter.h"

using namespace sy::emit;

Emitter::Emitter(air::Project &&project, LLVMContext &ctx)
	: project(std::move(project)), ctx(ctx), builder(ctx) {}

std::unique_ptr<Module> Emitter::emit() {
	std::unique_ptr<Module> module = std::make_unique<Module>("sy", ctx);

	FunctionType *mainFuncType = FunctionType::get(Type::getVoidTy(ctx), {}, false);
	Function *mainFunc = Function::Create(mainFuncType, Function::ExternalLinkage, "main", module.get());
	BasicBlock *entry = BasicBlock::Create(ctx, "entry", mainFunc);
	builder.SetInsertPoint(entry);

	for (std::unique_ptr<air::Stmt> &node : project.stmts) {
		builder.CreateStore(emit(node.get()), builder.CreateAlloca(Type::getInt32Ty(ctx)));
	}

	return std::move(module);
}

Value *Emitter::emit(air::Stmt *node) {
	return emit((air::Expr *) node);
}

Type *Emitter::emit(air::TypeId node) {
	air::Type &type = project.types[node];
	if (type.kind == air::Type::SInt) {
		return Type::getInt32Ty(ctx);
	} else if (type.kind == air::Type::UInt) {
		return Type::getInt32Ty(ctx);
	} else if (type.kind == air::Type::Float) {
		return Type::getFloatTy(ctx);
	} else {
		abort();
	}
}

Value *Emitter::emit(air::Expr *node) {
	switch (node->getKind()) {
	case air::Kind::BinOp:
		return emit((air::BinOp *) node);
	case air::Kind::Num:
		return emit((air::Num *) node);

	default:
		abort();
	}
}

Value *Emitter::emit(air::BinOp *node) {
	Value *left = emit(node->left.get());
	Value *right = emit(node->right.get());

	switch (node->opKind) {
	case air::BinOp::Add:
		return builder.CreateAdd(left, right);
	case air::BinOp::Sub:
		return builder.CreateSub(left, right);
	case air::BinOp::Mul:
		return builder.CreateMul(left, right);
	case air::BinOp::Div:
		return builder.CreateSDiv(left, right);

	default:
		abort();
	}
}

Value *Emitter::emit(air::Num *node) {
	switch (node->numConstant.kind) {
	case NumConstant::SInt:
		return ConstantInt::get(Type::getInt32Ty(ctx), node->numConstant.signedVal);
	case NumConstant::UInt:
		return ConstantInt::get(Type::getInt32Ty(ctx), node->numConstant.unsignedVal);
	case NumConstant::Float:
		return ConstantFP::get(Type::getFloatTy(ctx), node->numConstant.floatVal);

	default:
		abort();
	}
}

Type *Emitter::findType(air::TypeId node) {
	return types[node];
}