using System.Text;

namespace CommenSense;

using System;

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

	public static string Else(StmtAst? elze)
	{
		if (elze is null)
			return string.Empty;
		return $"\n{IncTab()}{elze}{DecTab()}";
	}

	public static string Mutableness(bool mutable)
	{
		if (mutable)
			return "mut ";
		return string.Empty;
	}
}

partial record ModuleAst
{ public override string ToString() => $"module '{name}': '{srcFile}' {{\n{IncTab()}{string.Join<StmtAst>("\n\n\t", members)}\n}}"; }


partial record TypeAst
{
	private string? _name;
	public string name => _name??= GenName();
	protected abstract string GenName();
}

partial record PtrTypeAst
{
	protected override string GenName() =>
		$"{Mutableness(mutable)}{baze}*";
	
	public override string ToString() => name;
}

partial record BaseTypeAst
{
	public string typeBase => token.text;

	protected override string GenName() =>
		typeBase;
	
	public override string ToString() => name;
}

partial record FuncTypeAst
{
	protected override string GenName() =>
		$"{retType} ({string.Join<TypeAst>(", ", paramTypes)}{PrintWithVaArgMaybeIHonestlyDontKnowWhatToCallThisFunction(vaArg)})";

	public override string ToString() =>
		name;
}

partial record ParamAst
{ public override string ToString() => $"{string.Join<string>(" @", metadata)} {type} {name} = {defaultExpr}"; }

partial record EnumValueAst
{
	public string name => token.text;

	public override string ToString() => $"{string.Join<string>(" @", metadata)} {name} = {initializer}";
}

partial record FieldAst
{
	public string name => token.text;

	public override string ToString() => $"@{string.Join<string>(" @", metadata)} {visibility} {type} {name} = {initializer}";
}


partial record EnumAst
{
	public string name => token.text;
	public override string ToString() => $"@{string.Join<string>(" @", metadata)} {visibility} enum {name}: {type} {{\n{IncTab()}{string.Join<EnumValueAst>("\n" + GetTabs(), values)}{DecTab()}\n{GetTabs()}}}";
	
}

partial record ClassAst
{
	public string name => token.text;

	// i can be ~0 (-1)
	public FieldInfo GetField(string name)
	{
		int i = Array.FindIndex(fields, field => field.name == name);
		bool mutable = false;
		if (i != -1)
			mutable = fields[i].metadata.Contains("mut");
		return new FieldInfo((uint) i, mutable);
	}

	// i can be ~0
	public FieldInfo GetFieldWithLvl(string name, Visibility permLvl)
	{
		FieldInfo field = GetField(name);
		if (field.idx == ~0U)
			return field;
		
		Visibility vis = fields[field.idx].visibility;
		if (permLvl is Visibility.LLVMHiddenVisibility)
			return field;
		if (permLvl is Visibility.LLVMProtectedVisibility && vis is not Visibility.LLVMHiddenVisibility)
			return field;
		if (permLvl is Visibility.LLVMDefaultVisibility && vis is Visibility.LLVMDefaultVisibility)
			return field;
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

	public override string ToString() => $"@{string.Join<string>(" @", metadata)} {visibility} class {name} {{\n{IncTab()}{string.Join<FieldAst>("\n" + GetTabs(), fields)}{"\n\n" + GetTabs()}{string.Join<FuncAst>("\n\n" + GetTabs(), funcs)}{DecTab()}\n{GetTabs()}}}";
}

partial record StructAst
{
	public string name => token.text;

	public FieldInfo GetField(string name)
	{
		int i = Array.FindIndex(fields, field => field.name == name);
		if (i != -1)
			throw new Exception("we ain't got dat field");
		return new FieldInfo((uint) i, fields[i].metadata.Contains("mut"));
	}

	public FieldInfo GetFieldWithLvl(string name, Visibility permLvl)
	{
		FieldInfo field = GetField(name);
		Visibility vis = fields[field.idx].visibility;
		if (permLvl is Visibility.LLVMHiddenVisibility)
			return field;
		if (permLvl is Visibility.LLVMProtectedVisibility && vis is not Visibility.LLVMHiddenVisibility)
			return field;
		if (permLvl is Visibility.LLVMDefaultVisibility && vis is Visibility.LLVMDefaultVisibility)
			return field;
		throw new Exception("Ya ain't got perms");
	}

	public override string ToString() => $"@{string.Join<string>(" @", metadata)} {visibility} struct {name} {{\n{IncTab()}{string.Join<FieldAst>("\n" + GetTabs(), fields)}{DecTab()}\n{GetTabs()}}}";
}


partial record UsingAst
{ public override string ToString() => $"{visibility} using {realType} as {alias}"; }

partial record LinkAst
{ public override string ToString() => $"{visibility} link {filename}"; }


partial record DeclFuncAst
{
	public string realName => token.text;
	
	public override string ToString() => $"@{string.Join<string>(" @", metadata)} {visibility} decl {retType} {realName}({string.Join<ParamAst>(", ", paramz)}{PrintWithVaArgMaybeIHonestlyDontKnowWhatToCallThisFunction(vaArg)}): '{asmName}'";
}

partial record FuncAst
{
	public string realName => token.text;

	public override string ToString() => $"@{string.Join<string>(" @", metadata)} {visibility} {retType} {realName}({string.Join<ParamAst>(", ", paramz)}{PrintWithVaArgMaybeIHonestlyDontKnowWhatToCallThisFunction(vaArg)}): '{asmName}' {{\n{IncTab()}{string.Join<StmtAst>($"\n{GetTabs()}", body)}{DecTab()}\n{GetTabs()}}}";
}

partial record DeclVarAst
{
	public string realName => token.text;
	public bool mutable => metadata.Contains("const");
	
	public override string ToString() => $"@{string.Join<string>(" @", metadata)} {visibility} decl {type} {realName}: '{asmName}'";
}

partial record VarAst
{
	public string realName => token.text;
	
	public override string ToString() => $"@{string.Join<string>(" @", metadata)} {visibility} {type} {realName}: '{asmName}' = {initializer}";
}


partial record IfStmtAst
{ public override string ToString() => $"if ({cond})\n{IncTab()}{then}{DecTab()}{Else(elze)}"; }

partial record WhileStmtAst
{ public override string ToString() => $"while ({cond})\n{IncTab()}{then}{DecTab()}"; }

partial record ForStmtAst
{ public override string ToString() => $"for ({init}\n{GetTabs()}{cond}\n{GetTabs()}{step})\n{IncTab()}{then}{DecTab()}"; }

partial record BlockStmtAst
{ public override string ToString() => $"{{\n{IncTab()}{string.Join<StmtAst>($"\n{GetTabs()}", stmts)}{DecTab()}\n{GetTabs()}}}"; }


partial record BreakStmtAst
{ public override string ToString() => $"break {depth}"; }

partial record BreakAllStmtAst
{ public override string ToString() => $"breakall"; }

partial record RetStmtAst
{ public override string ToString() => $"ret {expr}"; }

partial record ExprStmtAst
{ public override string ToString() => expr.ToString(); }

partial record ExprAst
{ public override string ToString() => "null"; }


partial record NullLiteralExprAst
{ public override string ToString() => "null"; }

partial record IntLiteralExprAst
{ public override string ToString() => $"{value}"; }

partial record BoolLiteralExprAst
{ public override string ToString() => $"{value}"; }

partial record StrLiteralExprAst
{ public override string ToString() => $"'{value}'"; }

partial record CStrLiteralExprAst
{ public override string ToString() => $"\"{value}\""; }

partial record CharLiteralExprAst
{ public override string ToString() => $"`{value}`"; }

partial record FloatLiteralExprAst
{ public override string ToString() => $"{value}"; }


partial record StallocExprAst
{ public override string ToString() => $"stalloc({size})"; }
partial record SizeofExprAst
{ public override string ToString() => $"sizeof({type})"; }
partial record CallExprAst
{ public override string ToString() => $"{ptr}({string.Join<ExprAst>(", ", args)})"; }

partial record FuncPtrAst
{
	public string funcName => token.text;
	
	public override string ToString() => $"&{funcName}";
}

partial record MemberExprAst
{
	public string memberName => token.text;
	
	public override string ToString() => $"{container}.{memberName}";
}

partial record VarExprAst
{
	public string varName => token.text;
	
	public override string ToString() => $"{varName}";
}


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


partial record PreExprAst
{ public override string ToString() => $"{op} {operand}"; }

partial record PostExprAst
{ public override string ToString() => $"{operand} {op}"; }

partial record BiExprAst
{ public override string ToString() => $"{op}\n{IncTab()}{left}\n{GetTabs()}{right}{DecTab()}"; }