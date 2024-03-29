﻿namespace CommenSense;

using Type = LLVMTypeRef;
using Value = LLVMValueRef;

partial class Builder
{
	Value BuildExpr(ExprAst ast)
	{
		if (ast is CStrLiteralExprAst cstrLiteral)
			return llBuilder.BuildGlobalString(cstrLiteral.value);
		if (ast is StrLiteralExprAst strLiteral)
			return BuildExpr(strLiteral);
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
			return Value.CreateConstReal(Type.Float, floatLiteral.value);

		if (ast is StallocExprAst stallocExpr)
		{
			Value array = llBuilder.BuildArrayAlloca(Type.Int8, BuildExpr(stallocExpr.size));
			array.TypeOf.SetMutable(true);
			return array;
		}

		if (ast is SizeofExprAst sizeofExpr)
			return BuildType(sizeofExpr.type).SizeOf;
		
		if (ast is FuncPtrAst funcExpr)
			return scope.FindFunc(funcExpr.funcName);
		if (ast is VarExprAst varExpr)
		{
			Value ptr = scope.FindVar(varExpr.varName);
			Value val = llBuilder.BuildLoad(ptr);
			val.SetMutable(ptr.IsMutable());
			return val;
		}
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

	Value BuildExpr(StrLiteralExprAst ast)
	{
		Value[] values = new Value[ast.value.Length + 1];
		for (int i = 0; i < ast.value.Length; i++)
			values[i] = Value.CreateConstInt(Type.Int16, ast.value[i]);
		values[^1] = Value.CreateConstInt(Type.Int16, 0);
		
		return Value.CreateConstArray(Type.Int16, values);
	}

	// Hacky Move
	Value BuildExpr(CallExprAst ast)
	{
		int add = 0;
		if (ast.ptr is MemberExprAst)
			add = 1;
		Value[] args = new Value[ast.args.Length + add];

		int CtnrMutable = -1;
		if (ast.ptr is MemberExprAst memAst)
		{
			args[0] = BuildPtr(memAst.container);
			CtnrMutable = args[0].IsMutable() ? 1 : 0;
		}

		for (int i = 0; i < ast.args.Length; i++)
			args[i + add] = BuildExpr(ast.args[i]);

		Value ptr;
		if (ast.ptr is FuncPtrAst fnPtr)
			ptr = Func(fnPtr.funcName);
		else if (ast.ptr is MemberExprAst memExpr)
			ptr = BuildExpr(memExpr, ast, Func);
		else
			ptr = BuildExpr(ast.ptr);

		for (int i = 0; i < ast.args.Length; i++)
		{
			if (ptr.IsAFunction != null && i < ptr.ParamsCount)
				args[i + add] = BuildCast(args[i + add], ptr.Params[i + add].TypeOf, ast.token);
			if (ptr.TypeOf.ElementType != null && i < ptr.TypeOf.ElementType.ParamTypes.Length)
				args[i + add] = BuildCast(args[i + add], ptr.TypeOf.ElementType.ParamTypes[i + add], ast.token);
		}

		return llBuilder.BuildCall(ptr, args);
		
		Value Func(string name)
		{
			List<Value> funcs = scope.FindFuncs(name);

			if (funcs.Count <= 1)
			{
				if (CtnrMutable == 0 && funcs[0].IsMutable())
					throw new Exception("cannot call mut func w/out mut ctnr");
				return funcs[0];
			}

			foreach (Value func in funcs)
			{
				if (CtnrMutable == 0 && func.IsMutable())
					continue;
					
				if (func.ParamsCount > args.Length)
					continue;
				
				if (func.ParamsCount < args.Length && !func.TypeOf.IsFunctionVarArg)
					continue;

				for (int i = 0; i < func.ParamsCount; i++)
					if (args[i].TypeOf.SameAs(func.TypeOf))
						break;

				return func;
			}

			throw new Exception("cannot find func");
			return null;
		}
	}

	Value BuildExpr(MemberExprAst ast)
	{
		Value container = BuildPtr(ast.container);
		Container ctnr = scope.GetCtnr(container.TypeOf.ElementType.StructName);
		FieldInfo field = ctnr.GetFieldWithLvl(ast.memberName, LLVMDefaultVisibility);
		if (field.idx == ~0U)
		{
			if (ctnr is ClassAst clazz)
				return scope.FindFunc(clazz.name + "." + clazz.funcs[clazz.GetFuncWithLvl(ast.memberName, LLVMDefaultVisibility)].realName);
			throw new Exception("we ain't got dat field");
		}
		
		return llBuilder.BuildLoad(llBuilder.BuildStructGEP(container, field.idx));
	}
	
	Value BuildExpr(MemberExprAst ast, CallExprAst callee, Func<string, Value> evalFunc)
	{
		Value container = BuildPtr(ast.container);
		Container ctnr = scope.GetCtnr(container.TypeOf.ElementType.StructName);
		FieldInfo field = ctnr.GetFieldWithLvl(ast.memberName, LLVMDefaultVisibility);
		if (field.idx == ~0U)
		{
			if (ctnr is ClassAst clazz)
				return evalFunc.Invoke(clazz.name + "." + clazz.funcs[clazz.GetFuncWithLvl(ast.memberName, LLVMDefaultVisibility)].realName);
			throw new Exception("we ain't got dat func");
		}
		
		return llBuilder.BuildLoad(llBuilder.BuildStructGEP(container, field.idx));
	}

	Value BuildExpr(ArrayExprAst ast)
	{
		Value firstEle = BuildExpr(ast.elements[0]);
		Type eleType = ast.eleType is null ? firstEle.TypeOf : BuildType(ast.eleType);
		Value array = llBuilder.BuildAlloca(Type.CreateArray(eleType, (uint) ast.elements.Length));
		Value ptr = llBuilder.BuildBitCast(array, Type.CreatePointer(eleType, 0));
		if (ast.elements.Length <= 0)
			return ptr;
		
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
		case LLVMAdd:
			return operand;
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
		if (CastVerifier.CastPrecedence(left.TypeOf, right.TypeOf) >
		    CastVerifier.CastPrecedence(right.TypeOf, left.TypeOf))
			type = right.TypeOf;

		if (ast.op is TokenKind kind)
		{
			switch (kind)
			{
			case TokenKind.And2:
			case TokenKind.Pipe2:
				return BuildLogic(kind, ast, left, right);

			default:
				left = BuildCast(left, type, ast.left.token);
				right = BuildCast(right, type, ast.right.token);
				
				Value ptr = BuildPtr(ast.left);
				return BuildAssign(ptr, ast.token, left, right);
			}
		}

		left = BuildCast(left, type, ast.left.token);
		right = BuildCast(right, type, ast.right.token);

		if (ast.op is LLVMIntPredicate _op)
			return BuildPred(_op, left, right);

		var op = (LLVMOpcode) ast.op;
		if (type.ElementType == default && type.IsFloat())
			op++;

		return llBuilder.BuildBinOp(op, left, right);
	}

	// TODO: fix
	Value BuildBi(Enum op, Token tok, Value left, Value right)
	{
		Type type = left.TypeOf;

		if (op is TokenKind kind)
		{
			switch (kind)
			{
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
		if (!ptr.IsMutable())
			throw new Exception("cannot modify const var");
		
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
			return llBuilder.BuildFCmp(iop.ToRealPredicate(), left, right);
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