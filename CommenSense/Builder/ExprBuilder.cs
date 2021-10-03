namespace CommenSense;

using Type = LLVMTypeRef;
using Value = LLVMValueRef;

partial class Builder
{
	Value BuildExpr(ExprAst ast)
	{
		if (ast is StrLiteralExprAst strLiteral)
			return llBuilder.BuildGlobalString(strLiteral.value);
		if (ast is CharLiteralExprAst charLiteral)
		{
			return Value.CreateConstInt(Type.CreateInt((uint) charLiteral.nBits), charLiteral.value);
		}
		if (ast is IntLiteralExprAst intLiteral)
		{
			Type type = (uint) intLiteral.value == intLiteral.value ? Type.Int32 : Type.Int64;
			return Value.CreateConstInt(type, intLiteral.value);
		}
		if (ast is BoolLiteralExprAst boolLiteral)
			return Value.CreateConstInt(Type.Int1, boolLiteral.value ? 1ul : 0ul);
		if (ast is FloatLiteralExprAst floatLiteral)
		{
			Type type = (float) floatLiteral.value == floatLiteral.value ? Type.Float : Type.Double;
			return Value.CreateConstReal(type, floatLiteral.value);
		}

		if (ast is FuncPtrAst funcExpr)
			return scope.Find(funcExpr.funcName);
		if (ast is VarExprAst varExpr)
			return llBuilder.BuildLoad(scope.Find(varExpr.varName));
		if (ast is CallExprAst callExpr)
			return BuildExpr(callExpr);
		if (ast is MemberExprAst memberExpr)
			return BuildExpr(memberExpr);

		if (ast is ArrayExprAst arrExpr)
			return BuildExpr(arrExpr);
		if (ast is IndexExprAst idxExpr)
			return BuildExpr(idxExpr);
		if (ast is CastExprAst castExpr)
			return BuildCast(BuildExpr(castExpr.value), BuildType(castExpr.to));
		if (ast is BitCastExprAst bitCastExpr)
			return llBuilder.BuildBitCast(BuildExpr(bitCastExpr.value), BuildType(bitCastExpr.to));

		if (ast is UnExprAst unExpr)
			return BuildExpr(unExpr);
		if (ast is BiExprAst biExpr)
			return BuildExpr(biExpr);
		if (ast.GetType() == typeof(ExprAst))
			return Value.CreateConstNull(Type.Int1);
		throw new Exception("Bob the builder can't build this ◁[<");
	}

	Value BuildExpr(CallExprAst ast)
	{
		Value ptr = BuildExpr(ast.ptr);
		Value[] args = new Value[ast.args.Length];
		for (int i = 0; i < args.Length; i++)
		{
			Value arg = BuildExpr(ast.args[i]);
			if (ptr.IsAFunction != null && i < ptr.ParamsCount)
				arg = BuildCast(arg, ptr.Params[i].TypeOf);
			args[i] = arg;
		}

		return llBuilder.BuildCall(ptr, args);
	}

	Value BuildExpr(MemberExprAst ast)
	{
		Value container = BuildPtr(ast.container);
		StructAst ztruct = scope.GetStruct(container.TypeOf.ElementType.StructName);
		uint i = ztruct.GetField(ast.memberName);
		return llBuilder.BuildLoad(llBuilder.BuildStructGEP(container, i));
	}

	Value BuildExpr(ArrayExprAst ast)
	{
		if (ast.elements.Length <= 0)
			return null;

		Value firstEle = BuildExpr(ast.elements[0]);
		Type eleType = ast.eleType is null ? firstEle.TypeOf : BuildType(ast.eleType);
		Value array = llBuilder.BuildAlloca(Type.CreateArray(eleType, (uint) ast.elements.Length));
		Value ptr = llBuilder.BuildBitCast(array, Type.CreatePointer(eleType, 0));
		Value elePtr = llBuilder.BuildInBoundsGEP(ptr, new Value[] { Value.CreateConstInt(Type.Int64, 0) });
		llBuilder.BuildStore(BuildCast(firstEle, eleType), elePtr);

		for (int i = 1; i < ast.elements.Length; i++)
		{
			elePtr = llBuilder.BuildInBoundsGEP(ptr, new Value[] { Value.CreateConstInt(Type.Int64, (ulong) i) });
			llBuilder.BuildStore(BuildCast(BuildExpr(ast.elements[i]), eleType), elePtr);
		}

		return ptr;
	}

	Value BuildExpr(IndexExprAst ast) =>
		llBuilder.BuildLoad(BuildPtr(ast));

	Value BuildExpr(UnExprAst ast)
	{
		switch (ast.op)
		{
		case TokenKind.And:
			return BuildPtr(ast.operand);
		}

		Value operand = BuildExpr(ast.operand);

		switch (ast.op)
		{
		case LLVMFNeg:
			return operand.TypeOf.IsFloat() ? llBuilder.BuildFNeg(operand) : llBuilder.BuildNeg(operand);
		case LLVMLoad:
			return llBuilder.BuildLoad(operand);

		default:
			throw new Exception("bob the builders cannt build (nor spell)");
		}
	}

	Value BuildExpr(BiExprAst ast)
	{
		if (ast.op is TokenKind.Eql)
		{
			Value ptr = BuildPtr(ast.left);
			Value val = BuildCast(BuildExpr(ast.right), ptr.TypeOf.ElementType);

			llBuilder.BuildStore(val, ptr);
			return val;
		}

		Value left = BuildExpr(ast.left);
		Value right = BuildExpr(ast.right);

		var op = (LLVMOpcode) ast.op;
		Type type = left.TypeOf;
		if (type.ElementType == default && type.IsFloat())
			op++;
		right = BuildCast(right, type);

		return llBuilder.BuildBinOp(op, left, right);
	}

	Value BuildStructExpr(Type type, StructAst ztruct)
	{
		Value ptr = llBuilder.BuildAlloca(type);

		for (uint i = 0; i < ztruct.fields.Length; i++)
		{
			Value ele = BuildCast(BuildExpr(ztruct.fields[i].initializer), type.StructElementTypes[i]);
			Value fieldPtr = llBuilder.BuildStructGEP(ptr, i);
			llBuilder.BuildStore(ele, fieldPtr);
		}

		return llBuilder.BuildLoad(ptr);
	}
}