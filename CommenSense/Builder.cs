namespace CommenSense;

using System.Xml.Linq;

using Type = LLVMTypeRef;
using Value = LLVMValueRef;

partial class Builder
{
	readonly ModuleAst module;
	readonly LLVMModuleRef llModule;
	readonly LLVMBuilderRef llBuilder;
	readonly Value printfFn;
	readonly Value fmt;

	Value currentFunc;

	public Builder(ModuleAst module)
	{
		this.module = module;
		llModule = LLVMModuleRef.CreateWithName(module.name);
		llBuilder = LLVMBuilderRef.Create(llModule.Context);
		scope = new Scope(this);

		printfFn = llModule.AddFunction("printf", Type.CreateFunction(Type.Void, new Type[] { Type.CreatePointer(Type.Int8, 0) }, true));

		const string fmtStr = "%i\n";
		Value[] values = new Value[fmtStr.Length + 1];
		for (int i = 0; i < fmtStr.Length; i++)
			values[i] = Value.CreateConstInt(Type.Int8, fmtStr[i]);
		values[^1] = Value.CreateConstInt(Type.Int8, 0);
		Value arr = Value.CreateConstArray(Type.Int8, values);

		Value str = llModule.AddGlobal(arr.TypeOf, string.Empty);
		str.Linkage = LLVMLinkage.LLVMPrivateLinkage;
		str.IsGlobalConstant = true;
		str.HasUnnamedAddr = true;
		str.Initializer = arr;
		fmt = llBuilder.BuildBitCast(str, Type.CreatePointer(Type.Int8, 0));
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
		else if (ast is VarAst var)
			Build(var);
		else if(ast is DeclFuncAst declFunc)
			Build(declFunc);
		else if (ast is DeclVarAst declVar)
			Build(declVar);
		else if (ast is ExprStmtAst exprStmt)
		{
			Value val = BuildExpr(exprStmt.expr);
			llBuilder.BuildCall(printfFn, new Value[] { fmt, val });
		}
		else
			throw new Exception("Bob the builder can't build this ◁[<");
	}

	void Build(FuncAst ast)
	{
		Type[] paramTypes = new Type[ast.paramz.Length];
		for (int i = 0; i < ast.paramz.Length; i++)
			paramTypes[i] = BuildType(ast.paramz[i].type);

		Type retType = BuildType(ast.retType);
		Type ty = Type.CreateFunction(retType, paramTypes);
		Value fn = llModule.AddFunction(ast.name, ty);
		scope.Define(ast.name, fn);
		LLVMBasicBlockRef entry = fn.AppendBasicBlock(string.Empty);
		llBuilder.PositionAtEnd(entry);
		currentFunc = fn;

		EnterScope();
		for (int i = 0; i < ast.paramz.Length; i++)
		{
			Value ptr = llBuilder.BuildAlloca(paramTypes[i]);
			llBuilder.BuildStore(fn.Params[i], ptr);
			scope.Define(ast.paramz[i].name, ptr);
		}
		foreach (StmtAst stmt in ast.body)
			Build(stmt);
		ExitScope();
		llBuilder.BuildRetVoid();
	}

	void Build(VarAst ast)
	{
		Type type = BuildType(ast.type);

		if (currentFunc == null)
		{
			Value var = llModule.AddGlobal(type, ast.name);
			var.Initializer = BuildCast(BuildExpr(ast.initializer), type);
		}
		else
		{
			Value var = llBuilder.BuildAlloca(type, ast.name);
			Value initializer = BuildCast(BuildExpr(ast.initializer), type);
			llBuilder.BuildStore(initializer, var);
			scope.Define(ast.name, var);
		}
	}

	void Build(DeclFuncAst ast)
	{
		Type[] paramTypes = new Type[ast.paramz.Length];
		for (int i = 0; i < ast.paramz.Length; i++)
			paramTypes[i] = BuildType(ast.paramz[i].type);

		Type retType = BuildType(ast.retType);
		Type ty = Type.CreateFunction(retType, paramTypes);
		Value fn = llModule.AddFunction(ast.name, ty);
		scope.Define(ast.name, fn);
	}

	Type BuildType(TypeAst ast)
	{
		Type type = ast.typeBase switch
		{
			"void" => Type.Void,
			"bool" => Type.Int1,
			"char" => Type.Int8,
			"int" => Type.Int32,

			_ => llModule.GetTypeByName(ast.typeBase),
		};

		for (int i = 0; i < ast.ptrCount; i++)
			type = Type.CreatePointer(type, 0);

		return type;
	}

	Value BuildExpr(ExprAst ast)
	{
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
			if (ptr.IsAFunction != null)
				arg = BuildCast(arg, ptr.Params[i].TypeOf);
			args[i] = arg;
		}

		return llBuilder.BuildCall(ptr, args);
	}

	Value BuildExpr(UnExprAst ast)
	{
		Value operand = BuildExpr(ast.operand);

		switch (ast.op)
		{
		case LLVMFNeg:
			return operand.TypeOf.IsFloat() ? llBuilder.BuildFNeg(operand) : llBuilder.BuildNeg(operand);

		default:
			throw new Exception("bob the builders cannt build (nor spell)");
		}
	}

	Value BuildExpr(BiExprAst ast)
	{
		Value left = BuildExpr(ast.left);
		Value right = BuildExpr(ast.right);

		var op = (LLVMOpcode) ast.op;
		Type type = left.TypeOf;
		if (type.ElementType == default && type.IsFloat())
			op++;
		right = BuildCast(right, type);

		return llBuilder.BuildBinOp(op, left, right);
	}

	Value BuildCast(Value val, Type to)
	{
		Type from = val.TypeOf;

		if (from.IsFloat() && to.IsFloat())
			return llBuilder.BuildFPCast(val, to);
		if (from.IsFloat() && !to.IsFloat())
			return llBuilder.BuildFPToSI(val, to);
		if (!from.IsFloat() && to.IsFloat())
			return llBuilder.BuildSIToFP(val, to);

		if (from.IsPtr() && to.IsPtr())
			return llBuilder.BuildPointerCast(val, to);
		if (from.IsPtr() && !to.IsPtr())
			return llBuilder.BuildPtrToInt(val, to);
		if (!from.IsPtr() && to.IsPtr())
			return llBuilder.BuildIntToPtr(val, to);

		return llBuilder.BuildIntCast(val, to);
	}
}