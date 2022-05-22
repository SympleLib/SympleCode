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
	mainFunc = Function::Create(mainFuncType, GlobalValue::ExternalLinkage, 0, "main", module);

	BasicBlock *entry = BasicBlock::Create(ctx, "entry", mainFunc);
	builder.SetInsertPoint(entry);

	for (AstNode *node: ast)
		Emit(node);

	return module;
}

void Emitter::Emit(AstNode *node) {
	ExprAst *expr = dynamic_cast<ExprAst *>(node);
	if (expr) {
		Emit(expr);
		return;
	}

	Emit((StmtAst *) node);
}

void Emitter::Emit(StmtAst *node) {
	switch (node->getKind()) {
	case AstKind::ReturnStmt:
		Emit((ReturnStmtAst *) node);
		break;
	}
}

void Emitter::Emit(ReturnStmtAst *node) {
	if (node->value) {
		builder.CreateRet(Emit(node->value));
		return;
	}

	builder.CreateRetVoid();
}

Value *Emitter::Emit(ExprAst *node) {
	switch (node->getKind()) {
	case AstKind::BinaryExpr:
		return Emit((BinaryExprAst *) node);
	case AstKind::VariableExpr:
		return Emit((VariableExprAst *) node);
	case AstKind::IntLiteral:
		return Emit((LiteralAst *) node);
	default:
		return nullptr;
	}
}

Value *Emitter::Emit(BinaryExprAst *node) {
	Value *left, *right;
	right = Emit(node->right);

	switch (node->op) {
	case BinaryOp::Add:
		left = Emit(node->left);
		return builder.CreateAdd(left, right);
	case BinaryOp::Sub:
		left = Emit(node->left);
		return builder.CreateSub(left, right);

	case BinaryOp::Assign:
		left = EmitRef(node->left);
		return builder.CreateStore(right, left);

	default:
		return nullptr;
	}
}

Value *Emitter::Emit(VariableExprAst *node) {
	if (vars.contains(node->name))
		return builder.CreateLoad(builder.getInt32Ty(), vars[node->name]);

	Value *var = builder.CreateAlloca(builder.getInt32Ty());
	vars[node->name] = var;
	return builder.CreateLoad(builder.getInt32Ty(), var);
}


Value *Emitter::EmitRef(ExprAst *node) {
	switch (node->getKind()) {
	case AstKind::VariableExpr:
		return EmitRef((VariableExprAst *) node);

	default:
		return nullptr;
	}
}

Value *Emitter::EmitRef(VariableExprAst *node) {
	if (vars.contains(node->name))
		return vars[node->name];

	Value *var = builder.CreateAlloca(builder.getInt32Ty());
	vars[node->name] = var;
	return var;
}


Value *Emitter::Emit(LiteralAst *node) {
	AstKind kind = node->getKind();

	if (kind == AstKind::IntLiteral) {
		IntLiteralAst *intNode = (IntLiteralAst *) node;
		return builder.getInt32(intNode->value);
	}

	return nullptr;
}