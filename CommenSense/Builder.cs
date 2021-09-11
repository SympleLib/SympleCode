namespace CommenSense;

class Builder
{
	readonly Ast ast;
	readonly LLVMBuilderRef llBuilder;

	public Builder(Ast ast)
	{
		this.ast = ast;
		llBuilder = LLVMBuilderRef.Create(LLVMContextRef.Global);
	}

	public LLVMValueRef BuildExpr(ExprAst ast)
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