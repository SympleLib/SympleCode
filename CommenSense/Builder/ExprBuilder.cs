namespace CommenSense;

using Type = LLVMTypeRef;
using Value = LLVMValueRef;

partial class Builder
{
	Value BuildExpr(ExprAst ast)
	{
		if (ast is StrLiteralExprAst strLiteral)
			return BuildCast(llBuilder.BuildGlobalString(strLiteral.value), Type.CreatePointer(Type.Int8, 0), strLiteral.token);
		if (ast is CharLiteralExprAst charLiteral)
		{
			return Value.CreateConstInt(Type.CreateInt((uint) charLiteral.nBits), charLiteral.value);
		}

		if (ast is NullLiteralExprAst nullLiteral)
		{
			return Value.CreateConstNull(Type.CreatePointer(Type.Int8, 0));
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
		if (ast is GroupExprAst groupExpr)
			return BuildExpr(groupExpr);
		if (ast is IndexExprAst idxExpr)
			return BuildExpr(idxExpr);
		if (ast is CastExprAst castExpr)
			return BuildCast(BuildExpr(castExpr.value), BuildType(castExpr.to), null);
		if (ast is BitCastExprAst bitCastExpr)
			return llBuilder.BuildBitCast(BuildExpr(bitCastExpr.value), BuildType(bitCastExpr.to));

		if (ast is PreExprAst preExpr)
			return BuildExpr(preExpr);
		if (ast is PostExprAst postExpr)
			return BuildExpr(postExpr);
		if (ast is BiExprAst biExpr)
			return BuildExpr(biExpr);
		if (ast.GetType() == typeof(ExprAst))
			return Value.CreateConstNull(Type.Int1);
		throw new Exception("Bob the builder can't build this ◁[<");
	}

	// Hacky Move
	Value BuildExpr(CallExprAst ast)
	{
		Value ptr = BuildExpr(ast.ptr);

		int add = 0;
		if (ast.ptr is MemberExprAst)
			add = 1;
		Value[] args = new Value[ast.args.Length + add];
		if (ast.ptr is MemberExprAst memAst)
			args[0] = BuildPtr(memAst.container);

		for (int i = 0; i < ast.args.Length; i++)
		{
			Value arg = BuildExpr(ast.args[i]);
			if (ptr.IsAFunction != null && i < ptr.ParamsCount)
				arg = BuildCast(arg, ptr.Params[i + add].TypeOf, ast.token);
			args[i + add] = arg;
		}

		return llBuilder.BuildCall(ptr, args);
	}

	Value BuildExpr(MemberExprAst ast)
	{
		Value container = BuildPtr(ast.container);
		Container ctnr = scope.GetCtnr(container.TypeOf.ElementType.StructName);
		uint i = ctnr.GetFieldWithLvl(ast.memberName, LLVMDefaultVisibility);
		if (i == ~0U)
		{
			if (ctnr is ClassAst clazz)
				return scope.Find(clazz.name + "." + clazz.funcs[clazz.GetFuncWithLvl(ast.memberName, LLVMDefaultVisibility)].realName);
			throw new Exception("we ain't got dat field");
		}
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
		llBuilder.BuildStore(BuildCast(firstEle, eleType, ast.token), elePtr);

		for (int i = 1; i < ast.elements.Length; i++)
		{
			elePtr = llBuilder.BuildInBoundsGEP(ptr, new Value[] { Value.CreateConstInt(Type.Int64, (ulong) i) });
			llBuilder.BuildStore(BuildCast(BuildExpr(ast.elements[i]), eleType, ast.token), elePtr);
		}

		return ptr;
	}

	Value BuildExpr(GroupExprAst ast)
	{
		Type type = BuildType(ast.groupType);
		Value ptr = llBuilder.BuildAlloca(type);

		for (uint i = 0; i < ast.members.Length; i++)
		{
			Value ele = BuildCast(BuildExpr(ast.members[i]), type.StructElementTypes[i], ast.token);
			Value fieldPtr = llBuilder.BuildStructGEP(ptr, i);
			llBuilder.BuildStore(ele, fieldPtr);
		}

		return llBuilder.BuildLoad(ptr);
	}

	Value BuildExpr(IndexExprAst ast) =>
		llBuilder.BuildLoad(BuildPtr(ast));

	Value BuildExpr(PreExprAst ast)
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

	Value BuildExpr(PostExprAst ast)
	{
		Value ptr = BuildPtr(ast.operand);
		Value left = BuildExpr(ast.operand);
		Value right = Value.CreateConstInt(Type.Int32, 1);
		Value val;

		Type type = left.TypeOf;
		right = BuildCast(right, type, null);

		// TODO: fix
		if (ast.op is TokenKind.Bang2)
			val = llBuilder.BuildNot(left);
		else
		{
			var op = (LLVMOpcode) ast.op;
			if (type.ElementType == default && type.IsFloat())
				op++;

			val = llBuilder.BuildBinOp(op, left, right);
		}

		llBuilder.BuildStore(val, ptr);
		return val;
	}

	Value BuildExpr(BiExprAst ast)
	{
		Value left = BuildExpr(ast.left);
		Value right = BuildExpr(ast.right);

		Type type = left.TypeOf;

		if (ast.op is TokenKind kind)
		{
			switch (kind)
			{
			case TokenKind.Star2:
				return llBuilder.BuildCall(llModule.GetNamedFunction("powi"), new Value[] { left, right });

			case TokenKind.And2:
			case TokenKind.Pipe2:
				return BuildLogic(kind, ast, left, right);

			default:
				right = BuildCast(right, type, ast.right.token);
				Value ptr = BuildPtr(ast.left);
				return BuildAssign(ptr, ast.token, left, right);
			}
		}

		right = BuildCast(right, type, ast.right.token);

		if (ast.op is LLVMIntPredicate _op)
			return BuildPred(_op, left, right);

		var op = (LLVMOpcode) ast.op;
		if (type.ElementType == default && type.IsFloat())
			op++;

		return llBuilder.BuildBinOp(op, left, right);
	}

	Value BuildBi(Enum op, Token tok, Value left, Value right)
	{
		Type type = left.TypeOf;

		if (op is TokenKind kind)
		{
			switch (kind)
			{
			case TokenKind.Star2:
				return llBuilder.BuildCall(llModule.GetNamedFunction("powi"), new Value[] { left, right });

			default:
				throw new NotImplementedException();
			}
		}

		right = BuildCast(right, type, tok);

		if (op is LLVMIntPredicate _op)
			return BuildPred(_op, left, right);

		var xop = (LLVMOpcode) op;
		if (type.ElementType == default && type.IsFloat())
			xop++;

		return llBuilder.BuildBinOp(xop, left, right);
	}

	Value BuildLogic(TokenKind op, BiExprAst ast, Value left, Value right)
	{
		left = BuildCast(left, Type.Int1, ast.left.token);
		right = BuildCast(right, Type.Int1, ast.right.token);

		switch (op)
		{
		case TokenKind.And2:
			return llBuilder.BuildAnd(left, right);
		case TokenKind.Pipe2:
			return llBuilder.BuildOr(left, right);

		default:
			throw new NotImplementedException();
		}
	}

	Value BuildAssign(Value ptr, Token tok, Value left, Value right)
	{
		if (tok.kind is TokenKind.Eql)
		{
			llBuilder.BuildStore(right, ptr);
			return right;
		}

		Enum vop = tok.kind switch
		{
			TokenKind.PlusEql => LLVMAdd,
			TokenKind.MinusEql => LLVMSub,
			TokenKind.StarEql => LLVMMul,
			TokenKind.SlashEql => LLVMSDiv,
			TokenKind.PercentEql => LLVMSRem,
			
			TokenKind.AndEql => LLVMAnd,
			TokenKind.PipeEql => LLVMOr,
			TokenKind.CarotEql => LLVMXor,

			TokenKind.Star2Eql => TokenKind.Star2,
			TokenKind.Left2Eql => LLVMShl,
			TokenKind.Right2Eql => LLVMAShr,

			_ => throw new NotImplementedException(),
		};

		Value val = BuildBi(vop, tok, left, right);
		llBuilder.BuildStore(val, ptr);
		return val;
	}

	Value BuildPred(LLVMIntPredicate iop, Value left, Value right)
	{
		if (left.TypeOf.IsFloat())
		{
			// TODO
			throw new NotImplementedException();
		}

		return llBuilder.BuildICmp(iop, left, right);
	}

	Value BuildCtnrExpr(Type type, Container ctnr)
	{
		Value ptr = llBuilder.BuildAlloca(type);

		for (uint i = 0; i < ctnr.fields.Length; i++)
		{
			Value ele = BuildCast(BuildExpr(ctnr.fields[i].initializer), type.StructElementTypes[i], null);
			Value fieldPtr = llBuilder.BuildStructGEP(ptr, i);
			llBuilder.BuildStore(ele, fieldPtr);
		}

		return llBuilder.BuildLoad(ptr);
	}
}