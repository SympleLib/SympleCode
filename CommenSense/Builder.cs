namespace CommenSense;

using Type = LLVMTypeRef;
using Value = LLVMValueRef;

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
		LLVMBasicBlockRef entry = fn.AppendBasicBlock(string.Empty);
		llBuilder.PositionAtEnd(entry);

		foreach (StmtAst stmt in ast.body)
			Build(stmt);
	}

	Type BuildType(TypeAst ast)
	{
		Type type = ast.typeBase switch
		{
			"void" => Type.Void,

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
		if (ast is FloatLiteralExprAst floatLiteral)
		{
			Type type = (float) floatLiteral.value == floatLiteral.value ? Type.Float : Type.Double;
			return Value.CreateConstReal(type, floatLiteral.value);
		}
		if (ast is BiExprAst biExpr)
			return BuildExpr(biExpr);
		throw new Exception("Bob the builder can't build this ◁[<");
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
			return llBuilder.BuildBitCast(val, to);

		return llBuilder.BuildIntCast(val, to);
	}
}