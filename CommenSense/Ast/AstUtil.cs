namespace CommenSense;

using System;
using System.Linq;

using LLVMSharp;

using static _;

static class _
{
	public static int tabCount;
	public static string IncTab() =>
		new string('\t', ++tabCount);
	public static string GetTabs() =>
		new string('\t', tabCount);
	public static string DecTab()
	{
		tabCount--;
		return string.Empty;
	}

	public static string ArrayThing(TypeAst? eleType)
	{
		if (eleType is null)
			return string.Empty;
		return eleType + " ";
	}

	public static string PrintWithVaArgMaybeIHonestlyDontKnowWhatToCallThisFunction(bool vaArg)
	{
		if (vaArg)
			return ", ...";
		return string.Empty;
	}
}

partial record ModuleAst
{ public override string ToString() => $"module '{name}' links {string.Join<string>(", ", links)} {{\n{IncTab()}{string.Join<StmtAst>("\n\n\t", members)}\n}}"; }


partial record BaseTypeAst
{ public override string ToString() => $"{typeBase}{new string('*', ptrCount)}"; }

partial record FuncTypeAst
{ public override string ToString() => $"{retType} ({string.Join<TypeAst>(", ", paramTypes)}{PrintWithVaArgMaybeIHonestlyDontKnowWhatToCallThisFunction(vaArg)}){new string('*', ptrCount)}"; }

partial record ParamAst
{ public override string ToString() => $"{type} {name} = {defaultExpr}"; }

partial record FieldAst
{ public override string ToString() => $"{visibility} {type} {name} = {initializer}"; }


partial record ClassAst
{
	// i can be ~0 (-1)
	public uint GetField(string name)
	{
		int i = Array.FindIndex(fields, field => field.name == name);
		return (uint) i;
	}

	// i can be ~0
	public uint GetFieldWithLvl(string name, Visibility permLvl)
	{
		uint i = GetField(name);
		if (i == ~0U)
			return i;
		Visibility vis = fields[i].visibility;
		if (permLvl is Visibility.LLVMHiddenVisibility)
			return i;
		if (permLvl is Visibility.LLVMProtectedVisibility && vis is not Visibility.LLVMHiddenVisibility)
			return i;
		if (permLvl is Visibility.LLVMDefaultVisibility && vis is Visibility.LLVMDefaultVisibility)
			return i;
		throw new Exception("Ya ain't got perms");
	}


	// i cant be ~0
	public uint GetFunc(string name)
{
		int i = Array.FindIndex(funcs, func => func.realName == name);
		if (i == -1)
			throw new Exception("we ain't got dat func");
		return (uint) i;
	}

	// i cant be ~0
	public uint GetFuncWithLvl(string name, Visibility permLvl)
	{
		uint i = GetFunc(name);
		Visibility vis = funcs[i].visibility;
		if (permLvl is Visibility.LLVMHiddenVisibility)
			return i;
		if (permLvl is Visibility.LLVMProtectedVisibility && vis is not Visibility.LLVMHiddenVisibility)
			return i;
		if (permLvl is Visibility.LLVMDefaultVisibility && vis is Visibility.LLVMDefaultVisibility)
			return i;
		throw new Exception("Ya ain't got perms");
	}

	public override string ToString() => $"{visibility} class {name} {{\n{IncTab()}{string.Join<FieldAst>("\n" + GetTabs(), fields)}{"\n\n" + GetTabs()}{string.Join<FuncAst>("\n\n" + GetTabs(), funcs)}{DecTab()}\n{GetTabs()}}}";
}

partial record StructAst
{
	public uint GetField(string name)
	{
		int i = Array.FindIndex(fields, field => field.name == name);
		if (i == -1)
			throw new Exception("we ain't got dat field");
		return (uint) i;
	}

	public uint GetFieldWithLvl(string name, Visibility permLvl)
	{
		uint i = GetField(name);
		Visibility vis = fields[i].visibility;
		if (permLvl is Visibility.LLVMHiddenVisibility)
			return i;
		if (permLvl is Visibility.LLVMProtectedVisibility && vis is not Visibility.LLVMHiddenVisibility)
			return i;
		if (permLvl is Visibility.LLVMDefaultVisibility && vis is Visibility.LLVMDefaultVisibility)
			return i;
		throw new Exception("Ya ain't got perms");
	}

	public override string ToString() => $"{visibility} struct {name} {{\n{IncTab()}{string.Join<FieldAst>("\n" + GetTabs(), fields)}{DecTab()}\n{GetTabs()}}}";
}


partial record UsingAst
{ public override string ToString() => $"using {realType} as {alias}"; }

partial record LinkAst
{ public override string ToString() => $"link {filename}"; }


partial record DeclFuncAst
{ public override string ToString() => $"{visibility} decl {retType} {realName}({string.Join<ParamAst>(", ", paramz)}{PrintWithVaArgMaybeIHonestlyDontKnowWhatToCallThisFunction(vaArg)}): '{asmName}'"; }

partial record FuncAst
{ public override string ToString() => $"{visibility} {retType} {realName}({string.Join<ParamAst>(", ", paramz)}{PrintWithVaArgMaybeIHonestlyDontKnowWhatToCallThisFunction(vaArg)}): '{asmName}' {{\n{IncTab()}{string.Join<StmtAst>($"\n{GetTabs()}", body)}{DecTab()}\n{GetTabs()}}}"; }

partial record DeclVarAst
{ public override string ToString() => $"{visibility} decl {type} {realName}: '{asmName}'"; }

partial record VarAst
{ public override string ToString() => $"{visibility} {type} {realName}: '{asmName}' = {initializer}"; }


partial record RetStmtAst
{ public override string ToString() => $"ret {expr}"; }

partial record ExprStmtAst
{ public override string ToString() => expr.ToString(); }

partial record ExprAst
{ public override string ToString() => "null"; }


partial record IntLiteralExprAst
{ public override string ToString() => $"{value}"; }

partial record BoolLiteralExprAst
{ public override string ToString() => $"{value}"; }

partial record StrLiteralExprAst
{ public override string ToString() => $"'{value}'"; }

partial record CharLiteralExprAst
{ public override string ToString() => $"`{value}`"; }

partial record FloatLiteralExprAst
{ public override string ToString() => $"{value}"; }


partial record CallExprAst
{ public override string ToString() => $"{ptr}({string.Join<ExprAst>(", ", args)})"; }

partial record FuncPtrAst
{ public override string ToString() => $"&{funcName}"; }

partial record MemberExprAst
{ public override string ToString() => $"{container}.{memberName}"; }

partial record VarExprAst
{ public override string ToString() => $"{varName}"; }


partial record ArrayExprAst
{ public override string ToString() => $"{ArrayThing(eleType)}[ {string.Join<ExprAst>(", ", elements)} ]"; }

partial record GroupExprAst
{ public override string ToString() => $"{ArrayThing(groupType)}{{ {string.Join<ExprAst>(", ", members)} }}"; }

partial record IndexExprAst
{ public override string ToString() => $"{ptr}[{index}]"; }

partial record CastExprAst
{ public override string ToString() => $"{value} to {to}"; }

partial record BitCastExprAst
{ public override string ToString() => $"{value} as {to}"; }


partial record UnExprAst
{ public override string ToString() => $"{op} {operand}"; }

partial record BiExprAst
{ public override string ToString() => $"{op}\n{IncTab()}{left}\n{GetTabs()}{right}{DecTab()}"; }