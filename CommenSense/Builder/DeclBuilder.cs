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
		else if (ast is FuncAst func)
			Decl(func);
		else if (ast is VarAst var)
			Decl(var);
		else if (ast is StructAst ztruct)
			Decl(ztruct);
		else if (ast is ClassAst clazz)
			Decl(clazz);
	}

	Value Decl(DeclVarAst ast)
	{
		Type type = BuildType(ast.type);
		Value var = llModule.AddGlobal(type, ast.asmName);
		var.Visibility = ast.visibility;
		scope.Define(ast.realName, var);
		return var;
	}

	Value Decl(FuncAst ast, string prefix = "")
	{
		Type[] paramTypes = new Type[ast.paramz.Length];
		for (int i = 0; i < ast.paramz.Length; i++)
			paramTypes[i] = BuildType(ast.paramz[i].type);

		Type retType = BuildType(ast.retType);
		Type type = Type.CreateFunction(retType, paramTypes, ast.vaArg);
		Value func = llModule.AddFunction(ast.asmName, type);
		func.Visibility = ast.visibility;
		// func.FunctionCallConv = (uint) ast.conv;
		scope.Define(prefix + ast.realName, func);
		return func;
	}

	Value Decl(VarAst ast)
	{
		if (currentFunc != null)
			return null;

		Type type = BuildType(ast.type);
		Value var = llModule.AddGlobal(type, ast.asmName);
		var.Visibility = ast.visibility;
		scope.Define(ast.realName, var);
		return var;
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
			Decl(func, ast.prefix);
	}
}