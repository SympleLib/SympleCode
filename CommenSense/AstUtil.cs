namespace CommenSense;

using System.Linq;

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
		return "";
	}

	public static string PrintWithVaArgMaybeIHonestlyDontKnowWhatToCallThisFunction(bool vaArg)
	{
		if (vaArg)
			return ", ...";
		return string.Empty;
	}
}

partial record ModuleAst
{ public override string ToString() => $"module `{name}`: [\n{IncTab()}{string.Join<StmtAst>("\n\n\t", members)}\n]"; }


partial record TypeAst
{ public override string ToString() => $"{typeBase}{new string('*', ptrCount)}"; }

partial record ParamAst
{ public override string ToString() => $"{type} {name} = {defaultExpr}"; }

partial record FieldAst
{ public override string ToString() => $"{vis} {type} {name} = {initializer}"; }


partial record StructAst
{ public override string ToString() => $"{vis} struct {name} {{\n{IncTab()}{string.Join<FieldAst>(",\n" + GetTabs(), fields)}{DecTab()}\n{GetTabs()}}}"; }


partial record DeclFuncAst
{ public override string ToString() => $"{vis} decl {retType} {name}[{string.Join<ParamAst>(", ", paramz)}{PrintWithVaArgMaybeIHonestlyDontKnowWhatToCallThisFunction(vaArg)}]"; }

partial record FuncAst
{ public override string ToString() => $"{vis} {retType} {name}[{string.Join<ParamAst>(", ", paramz)}{PrintWithVaArgMaybeIHonestlyDontKnowWhatToCallThisFunction(vaArg)}] {{\n{IncTab()}{string.Join<StmtAst>($"\n{GetTabs()}", body)}{DecTab()}\n{GetTabs()}}}"; }

partial record DeclVarAst
{ public override string ToString() => $"{vis} decl {type} {name}"; }

partial record VarAst
{ public override string ToString() => $"{vis} {type} {name} = {initializer}"; }


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
{ public override string ToString() => $"{ptr}[{string.Join<ExprAst>(", ", args)}]"; }

partial record FuncPtrAst
{ public override string ToString() => $"&{funcName}"; }

partial record VarExprAst
{ public override string ToString() => $"{varName}"; }


partial record UnExprAst
{ public override string ToString() => $"{op} {operand}"; }

partial record BiExprAst
{ public override string ToString() => $"{op}\n{IncTab()}{left}\n{GetTabs()}{right}{DecTab()}"; }