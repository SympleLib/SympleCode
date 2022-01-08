﻿namespace CommenSense;

using Type = LLVMTypeRef;
using Value = LLVMValueRef;

partial class Builder
{
	Type BuildType(TypeAst ast)
	{
		if (ast is BaseTypeAst baseType)
			return BuildType(baseType);
		if (ast is FuncTypeAst funcType)
			return BuildType(funcType);
		if (ast is PtrTypeAst ptrType)
		{
			Type baze = BuildType(ptrType.baze);
			Type type = Type.CreatePointer(baze, 0);
			type.SetMutable(ptrType.mutable);
			return type;
		}

		throw new Exception("type no exist");
	}

	Type BuildType(BaseTypeAst ast)
	{
		Type type = ast.typeBase switch
		{
			"void" => Type.Void,
			"bool" => Type.Int1,
			"char" => Type.Int16,
			"byte" => Type.Int8,
			"int" => Type.Int32,
			"long" => Type.Int64,
			"float" => Type.Float,
			"double" => Type.Double,

			_ => scope.FindType(ast.typeBase),
		};

		return type;
	}

	Type BuildType(FuncTypeAst ast)
	{
		Type[] paramTypes = new Type[ast.paramTypes.Length];
		for (int i = 0; i < paramTypes.Length; i++)
			paramTypes[i] = BuildType(ast.paramTypes[i]);

		Type retType = BuildType(ast.retType);
		Type type = Type.CreateFunction(retType, paramTypes, ast.vaArg);
		return type;
	}

	Value BuildCast(Value val, Type to, Token? token)
	{
		Type from = val.TypeOf;

		bool signExt = false;
		if (token is not null && !CastVerifier.CastWorks(from, to, out signExt))
			BadCode.Report(new SyntaxError($"cant implicitly cast {from} to {to}", token!));

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
			Container ctnr = scope.GetCtnr(to.StructName);
			return BuildCtnrExpr(to, ctnr); // TODO: Class Constructors
		}
		if (signExt)
			return llBuilder.BuildZExtOrBitCast(val, to);
		return llBuilder.BuildIntCast(val, to);
	}
}