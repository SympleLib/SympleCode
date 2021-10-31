namespace CommenSense;

using System;

using Type = LLVMTypeRef;
using Value = LLVMValueRef;

partial class Builder
{
	readonly ModuleAst module;
	readonly LLVMModuleRef llModule;
	readonly LLVMBuilderRef llBuilder;

	static readonly Type uninitType = Type.CreateInt(1);

	Value currentFunc;
	readonly List<ModuleAst> links = new List<ModuleAst>();

	public Builder(ModuleAst[] modules, int index)
	{
		module = modules[index];
		links.AddRange(modules);

		llModule = LLVMModuleRef.CreateWithName(module.name);
		llBuilder = LLVMBuilderRef.Create(llModule.Context);
		scope = new Scope(this);
	}

	public LLVMModuleRef Build()
	{
		foreach (Container ctnr in module.ctnrs)
		{
			llModule.Context.CreateNamedStruct(ctnr.name);
			scope.Define(ctnr.name, ctnr);
		}

		foreach (ModuleAst link in links)
			foreach (Container ctnr in link.ctnrs)
			{
				llModule.Context.CreateNamedStruct(ctnr.name);
				scope.Define(ctnr.name, ctnr);
			}

		foreach (StmtAst member in module.members)
			Decl(member);

		foreach (ModuleAst link in links)
			if (link != module)
				foreach (StmtAst member in link.members)
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
		else if (ast is ClassAst clazz)
			Build(clazz);
		else if (ast is UsingAst) { }
		else if (ast is LinkAst) { }
		else if (ast is RetStmtAst retStmt)
		{
			Value expr = BuildExpr(retStmt.expr);
			if (currentFunc.TypeOf.ElementType.ReturnType == Type.Void)
				llBuilder.BuildRetVoid();
			else
				llBuilder.BuildRet(expr);
		}
		else if (ast is ExprStmtAst exprStmt)
			BuildExpr(exprStmt.expr);
		else
			throw new InvalidOperationException("Bob the builder can't build this ◁[<");
	}

	void Build(ClassAst ast)
	{
		Type type = llModule.GetTypeByName(ast.name);
		foreach (FuncAst func in ast.funcs)
			Build(func, ast, type);
	}

	void Build(FuncAst ast)
	{
		Value fn = scope.Find(ast.realName);
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
		{
			Build(stmt);
			// In case new func
			currentFunc = fn;
		}
		ExitScope();
		if (fn.TypeOf.ElementType.ReturnType == Type.Void && entry.Terminator == null)
			llBuilder.BuildRetVoid();
	}

	void Build(FuncAst ast, ClassAst clazz, Type clsType)
	{
		Value fn = scope.Find(clsType.StructName + "." + ast.realName);
		Type[] paramTypes = fn.TypeOf.ElementType.ParamTypes;
		LLVMBasicBlockRef entry = fn.AppendBasicBlock(string.Empty);
		llBuilder.PositionAtEnd(entry);
		currentFunc = fn;

		EnterScope();
		Value thiz = fn.Params[0];
		scope.Define("this", thiz);
		for (uint i = 0; i < clazz.fields.Length; i++)
		{
			Value ptr = llBuilder.BuildStructGEP(thiz, i);
			scope.Define(clazz.fields[i].name, ptr);
		}

		EnterScope();
		for (int i = 0; i < ast.paramz.Length; i++)
		{
			Value ptr = llBuilder.BuildAlloca(paramTypes[i + 1]);
			llBuilder.BuildStore(fn.Params[i + 1], ptr);
			scope.Define(ast.paramz[i].name, ptr);
		}

		foreach (StmtAst stmt in ast.body)
			Decl(stmt);
		foreach (StmtAst stmt in ast.body)
		{
			Build(stmt);
			// In case new func
			currentFunc = fn;
		}
		ExitScope();
		ExitScope();

		if (fn.TypeOf.ElementType.ReturnType == Type.Void && entry.Terminator == null)
			llBuilder.BuildRetVoid();
	}

	void Build(VarAst ast)
	{
		Type type = BuildType(ast.type);
		Value initializer = BuildCast(BuildExpr(ast.initializer), type);

		if (currentFunc == null)
		{
			Value var = scope.Find(ast.realName);
			var.Initializer = initializer;
		}
		else
		{
			Value var = llBuilder.BuildAlloca(type, ast.asmName);
			llBuilder.BuildStore(initializer, var);
			scope.Define(ast.realName, var);
		}
	}

	void Build(DeclFuncAst ast)
	{
		Type[] paramTypes = new Type[ast.paramz.Length];
		for (int i = 0; i < ast.paramz.Length; i++)
			paramTypes[i] = BuildType(ast.paramz[i].type);

		Type retType = BuildType(ast.retType);
		Type ty = Type.CreateFunction(retType, paramTypes, ast.vaArg);
		Value fn = llModule.AddFunction(ast.asmName, ty);
		// fn.FunctionCallConv = (uint) ast.conv;
		scope.Define(ast.realName, fn);
	}
}