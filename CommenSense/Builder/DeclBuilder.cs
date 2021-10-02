namespace CommenSense;

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
	}

	void Decl(DeclVarAst ast)
	{
		Type type = BuildType(ast.type);
		Value var = llModule.AddGlobal(type, ast.name);
		scope.Define(ast.name, var);
	}

	void Decl(FuncAst ast)
	{
		Type[] paramTypes = new Type[ast.paramz.Length];
		for (int i = 0; i < ast.paramz.Length; i++)
			paramTypes[i] = BuildType(ast.paramz[i].type);

		Type retType = BuildType(ast.retType);
		Type type = Type.CreateFunction(retType, paramTypes, ast.vaArg);
		Value func = llModule.AddFunction(ast.name, type);
		scope.Define(ast.name, func);
	}

	void Decl(VarAst ast)
	{
		if (currentFunc != null)
			return;

		Type type = BuildType(ast.type);
		Value var = llModule.AddGlobal(type, ast.name);
		scope.Define(ast.name, var);
	}

	void Decl(StructAst ast)
	{
		Type type = llModule.GetTypeByName(ast.name);
		Type[] elTypes = new Type[ast.fields.Length];
		for (int i = 0; i < elTypes.Length; i++)
			elTypes[i] = BuildType(ast.fields[i].type);
		type.StructSetBody(elTypes, false);
	}
}