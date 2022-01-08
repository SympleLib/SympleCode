namespace CommenSense;

using Value = LLVMValueRef;
using Type = LLVMTypeRef;

partial class Builder
{
	Value BuildPtr(ExprAst ast)
	{
		if (ast is FuncPtrAst funcExpr) // TODO: fix
			return scope.FindFunc(funcExpr.funcName);
		if (ast is VarExprAst varExpr)
			return scope.FindVar(varExpr.varName);
		if (ast is MemberExprAst memberExpr)
			return BuildPtr(memberExpr);
		if (ast is IndexExprAst idxExpr)
			return BuildPtr(idxExpr);
		if (ast is PreExprAst preExpr)
		{
			//if (unExpr.op is TokenKind.And)
			//	return BuildPtr(unExpr.operand);
			if (preExpr.op is LLVMLoad) 
				return BuildExpr(preExpr.operand);
		}
		throw new Exception("not a ptr D:{");
	}

	Value BuildPtr(IndexExprAst ast)
	{
		Value ptr = BuildExpr(ast.ptr);
		Value elePtr = llBuilder.BuildInBoundsGEP(ptr, new Value[] { BuildExpr(ast.index) });
		elePtr.SetMutable(ptr.TypeOf.IsMutable());
		return elePtr;
	}

	Value BuildPtr(MemberExprAst ast)
	{
		Value container = BuildPtr(ast.container);
		Container ctnr = scope.GetCtnr(container.TypeOf.ElementType.StructName);
		FieldInfo field = ctnr.GetField(ast.memberName);
		Value ptr = llBuilder.BuildStructGEP(container, field.idx);
		ptr.SetMutable(field.mutable || container.IsMutable());
		return ptr;
	}
}