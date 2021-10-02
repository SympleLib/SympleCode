namespace CommenSense;

using Value = LLVMValueRef;

partial class Builder
{
	Value BuildPtr(ExprAst ast)
	{
		if (ast is FuncPtrAst funcExpr)
			return scope.Find(funcExpr.funcName);
		if (ast is VarExprAst varExpr)
			return scope.Find(varExpr.varName);
		if (ast is MemberExprAst memberExpr)
			return BuildPtr(memberExpr);
		if (ast is UnExprAst unExpr)
		{
			//if (unExpr.op is TokenKind.And)
			//	return BuildPtr(unExpr.operand);
			if (unExpr.op is LLVMLoad)
				return BuildExpr(unExpr.operand);
		}
		throw new Exception("not a ptr D:{");
	}

	Value BuildPtr(MemberExprAst ast)
	{
		Value container = BuildPtr(ast.container);
		StructAst ztruct = scope.GetStruct(container.TypeOf.ElementType.StructName);
		uint i = ztruct.GetField(ast.memberName);
		return llBuilder.BuildStructGEP(container, i);
	}
}