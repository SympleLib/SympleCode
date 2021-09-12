﻿namespace CommenSense;

class Builder
{
	readonly ModuleAst module;
	readonly LLVMModuleRef llModule;
	readonly LLVMBuilderRef llBuilder;

	public Builder(ModuleAst module)
	{
		this.module = module;
		llModule = LLVMModuleRef.CreateWithName(module.name);
		llBuilder = LLVMBuilderRef.Create(llModule.Context);
	}

	public LLVMModuleRef Build()
	{
		foreach (StmtAst member in module.members)
			Build(member);
		return llModule;
	}

	void Build(StmtAst ast)
	{
		if (ast is FuncAst func)
			Build(func);
		else if (ast is ExprStmtAst exprStmt)
			llBuilder.BuildRet(BuildExpr(exprStmt.expr));
	}

	void Build(FuncAst ast)
	{
		LLVMTypeRef ty = LLVMTypeRef.CreateFunction(LLVMTypeRef.Void, Array.Empty<LLVMTypeRef>());
		LLVMValueRef fn = llModule.AddFunction(ast.name, ty);
		LLVMBasicBlockRef entry = fn.AppendBasicBlock(string.Empty);
		llBuilder.PositionAtEnd(entry);

		foreach (StmtAst stmt in ast.body)
			Build(stmt);
	}

	LLVMValueRef BuildExpr(ExprAst ast)
	{
		if (ast is IntLiteralExprAst intLiteral)
			return LLVMValueRef.CreateConstInt(LLVMTypeRef.Int32, intLiteral.value);
		if (ast is FloatLiteralExprAst floatLiteral)
			return LLVMValueRef.CreateConstReal(LLVMTypeRef.Float, floatLiteral.value);
		if (ast is BiExprAst biExpr)
			return llBuilder.BuildBinOp((LLVMOpcode) biExpr.op, BuildExpr(biExpr.left), BuildExpr(biExpr.right));
		throw new Exception("Bob the builder can't build this ◁[<");
	}
}