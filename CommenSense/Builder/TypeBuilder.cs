namespace CommenSense;

using Type = LLVMTypeRef;

partial class Builder
{
	Type NonNativeType(string name)
	{
		Type type = llModule.GetTypeByName(name);
		if (type.Handle != IntPtr.Zero)
			return type;

		throw new Exception("type ain't existensial");
	}

	Type BuildType(TypeAst ast)
	{
		if (ast is BaseTypeAst baseType)
			return BuildType(baseType);
		if (ast is FuncTypeAst funcType)
			return BuildType(funcType);

		throw new Exception("type no exist");
	}

	Type BuildType(BaseTypeAst ast)
	{
		Type type = ast.typeBase switch
		{
			"void" => Type.Void,
			"bool" => Type.Int1,
			"char" => Type.Int8,
			"int" => Type.Int32,

			_ => NonNativeType(ast.typeBase),
		};

		for (int i = 0; i < ast.ptrCount; i++)
			type = Type.CreatePointer(type, 0);

		return type;
	}

	Type BuildType(FuncTypeAst ast)
	{
		Type[] paramTypes = new Type[ast.paramTypes.Length];
		for (int i = 0; i < paramTypes.Length; i++)
			paramTypes[i] = BuildType(ast.paramTypes[i]);

		Type retType = BuildType(ast.retType);
		Type type = Type.CreateFunction(retType, paramTypes, ast.vaArg);

		for (int i = 0; i < ast.ptrCount; i++)
			type = Type.CreatePointer(type, 0);

		return type;
	}
}