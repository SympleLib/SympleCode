namespace CommenSense;

using Type = LLVMTypeRef;
using Value = LLVMValueRef;

partial class Builder
{
	readonly ModuleAst module;
	readonly LLVMModuleRef llModule;
	readonly LLVMBuilderRef llBuilder;

	static readonly Type uninitType = Type.CreateInt(1);

	Value currentFunc;

	public Builder(ModuleAst module)
	{
		this.module = module;
		llModule = LLVMModuleRef.CreateWithName(module.name);
		llBuilder = LLVMBuilderRef.Create(llModule.Context);
		scope = new Scope(this);
	}

	public LLVMModuleRef Build()
	{
		foreach (StructAst ztruct in module.structs)
		{
			llModule.Context.CreateNamedStruct(ztruct.name);
			scope.Define(ztruct.name, ztruct);
		}
		foreach (StmtAst member in module.members)
			Decl(member);
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
		else if (ast is DeclFuncAst declFunc)
			Build(declFunc);
		else if (ast is DeclVarAst) { }
		else if (ast is StructAst) { }
		else if (ast is ExprStmtAst exprStmt)
			BuildExpr(exprStmt.expr);
		else
			throw new Exception("Bob the builder can't build this ◁[<");
	}

	void Build(FuncAst ast)
	{
		Value fn = scope.Find(ast.name);
		Type[] paramTypes = fn.TypeOf.ElementType.ParamTypes;
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
			Decl(stmt);
		foreach (StmtAst stmt in ast.body)
			Build(stmt);
		ExitScope();
		llBuilder.BuildRetVoid();
	}

	void Build(VarAst ast)
	{
		Type type = BuildType(ast.type);
		Value initializer = BuildCast(BuildExpr(ast.initializer), type);

		if (currentFunc == null)
		{
			Value var = scope.Find(ast.name);
			var.Initializer = initializer;
		}
		else
		{
			Value var = llBuilder.BuildAlloca(type, ast.name);
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
		Type ty = Type.CreateFunction(retType, paramTypes, ast.vaArg);
		Value fn = llModule.AddFunction(ast.name, ty);
		scope.Define(ast.name, fn);
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

		if (from == uninitType && to.StructName != string.Empty)
		{
			StructAst ztruct = scope.GetStruct(to.StructName);
			return BuildStructExpr(to, ztruct);
		}
		return llBuilder.BuildIntCast(val, to);
	}
}