namespace CommenSense;

using System;

using Type = LLVMTypeRef;
using Value = LLVMValueRef;

partial class Builder
{
	void Decl(StmtAst ast)
	{
		if (ast is DeclVarAst declVar)
			Decl(declVar);
		else if (ast is DeclFuncAst declFunc)
			Decl(declFunc);
		else if (ast is FuncAst func)
			Decl(func);
		else if (ast is VarAst var)
			Decl(var);
		else if (ast is StructAst ztruct)
			Decl(ztruct);
		else if (ast is ClassAst clazz)
			Decl(clazz);
		else if (ast is UsingAst uzing)
			Decl(uzing);
	}

	void Decl(DeclVarAst ast)
	{
		if (llModule.GetNamedGlobal(ast.asmName) != null)
			return;
		
		Type type = BuildType(ast.type);
		Value var = llModule.AddGlobal(type, ast.asmName);
		if (ast.metadata.Contains("const"))
			var.IsGlobalConstant = true;
		scope.DefineVar(ast.realName, var);
	}

	void Decl(DeclFuncAst ast)
	{
		if (llModule.GetNamedFunction(ast.asmName) != null)
			return;

		Type[] paramTypes = new Type[ast.paramz.Length];
		for (int i = 0; i < ast.paramz.Length; i++)
			paramTypes[i] = BuildType(ast.paramz[i].type);

		Type retType = BuildType(ast.retType);
		Type ty = Type.CreateFunction(retType, paramTypes, ast.vaArg);
		Value fn = llModule.AddFunction(ast.asmName, ty);
		// fn.FunctionCallConv = (uint) ast.conv;
		if (ast.metadata.Contains("dllimport"))
			fn.DLLStorageClass = LLVMDLLStorageClass.LLVMDLLImportStorageClass;
		scope.DefineFunc(ast.realName, fn);
	}

	void Decl(FuncAst ast)
	{
		if (llModule.GetNamedFunction(ast.asmName) != null)
			return;

		Type[] paramTypes = new Type[ast.paramz.Length];
		for (int i = 0; i < ast.paramz.Length; i++)
			paramTypes[i] = BuildType(ast.paramz[i].type);

		Type retType = BuildType(ast.retType);
		Type type = Type.CreateFunction(retType, paramTypes, ast.vaArg);
		Value func = llModule.AddFunction(ast.asmName, type);
		func.Visibility = ast.visibility;
		if (ast.metadata.Contains("dllimport"))
			func.DLLStorageClass = LLVMDLLStorageClass.LLVMDLLImportStorageClass;
		// func.FunctionCallConv = (uint) ast.conv;
		scope.DefineFunc(ast.realName, func);
	}

	void Decl(FuncAst ast, Type clsType)
	{
		if (llModule.GetNamedFunction(ast.asmName) != null)
			return;

		Type[] paramTypes = new Type[ast.paramz.Length + 1];
		paramTypes[0] = Type.CreatePointer(clsType, 0);
		for (int i = 0; i < ast.paramz.Length; i++)
			paramTypes[i + 1] = BuildType(ast.paramz[i].type);

		Type retType = BuildType(ast.retType);
		Type type = Type.CreateFunction(retType, paramTypes, ast.vaArg);
		Value func = llModule.AddFunction(ast.asmName, type);
		func.Visibility = ast.visibility;
		if (ast.metadata.Contains("dllimport"))
			func.DLLStorageClass = LLVMDLLStorageClass.LLVMDLLImportStorageClass;
		// func.FunctionCallConv = (uint) ast.conv;
		scope.DefineFunc(clsType.StructName + "." + ast.realName, func);
	}

	void Decl(VarAst ast)
	{
		if (currentFunc != null)
			return;
		
		if (llModule.GetNamedGlobal(ast.asmName) != null)
			return;

		Type type = BuildType(ast.type);
		Value var = llModule.AddGlobal(type, ast.asmName);
		var.Visibility = ast.visibility;
		if (ast.metadata.Contains("dllimport"))
			var.DLLStorageClass = LLVMDLLStorageClass.LLVMDLLImportStorageClass;
		if (ast.metadata.Contains("const"))
			var.IsGlobalConstant = true;
		scope.DefineVar(ast.realName, var);
	}

	void Decl(StructAst ast)
	{
		Type type = llModule.GetTypeByName(ast.name);
		Type[] elTypes = new Type[ast.fields.Length];
		for (int i = 0; i < elTypes.Length; i++)
			elTypes[i] = BuildType(ast.fields[i].type);
		type.StructSetBody(elTypes, false);
	}

	void Decl(ClassAst ast)
	{
		Type type = llModule.GetTypeByName(ast.name);
		Type[] elTypes = new Type[ast.fields.Length];
		for (int i = 0; i < elTypes.Length; i++)
			elTypes[i] = BuildType(ast.fields[i].type);
		type.StructSetBody(elTypes, false);

		foreach (FuncAst func in ast.funcs)
			Decl(func, type);
	}

	void Decl(UsingAst ast)
	{
		Type realType = BuildType(ast.realType);
		scope.Define(ast.alias, realType);
	}
}