﻿namespace CommenSense;

using Value = LLVMValueRef;
using Type = LLVMTypeRef;

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
		return elePtr;
	}

	Value BuildPtr(MemberExprAst ast)
	{
		Value container = BuildPtr(ast.container);
		Container ctnr = scope.GetCtnr(container.TypeOf.ElementType.StructName);
		uint i = ctnr.GetField(ast.memberName);
		return llBuilder.BuildStructGEP(container, i);
	}
}