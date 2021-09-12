namespace CommenSense;

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
}

partial record ModuleAst
{ public override string ToString() => $"module `{name}`: [\n{IncTab()}{string.Join<StmtAst>(";\n\t", members)}\n]"; }


partial record TypeAst
{ public override string ToString() => $"{typeBase}{new string('*', ptrCount)}"; }

partial record ParamAst
{ public override string ToString() => $"{{ {type} {name} = {defaultExpr} }}"; }

partial record FieldAst
{ public override string ToString() => $"{{ {vis} {type} {name} = {initializer} }}"; }


partial record FuncAst
{ public override string ToString() => $"{vis} {retType} {name}[{string.Join<ParamAst>(", ", paramz)}] {{\n{IncTab()}{string.Join<StmtAst>($",\n{GetTabs()}", body)}{DecTab()}\n{GetTabs()}}}"; }

partial record ExprStmtAst
{ public override string ToString() => expr.ToString(); }


partial record IntLiteralExprAst
{ public override string ToString() => $"{value}"; }

partial record StrLiteralExprAst
{ public override string ToString() => $"`{value}`"; }

partial record FloatLiteralExprAst
{ public override string ToString() => $"{value}"; }


partial record UnExprAst
{ public override string ToString() => $"{op} {operand}"; }

partial record BiExprAst
{ public override string ToString() => $"{op}\n{IncTab()}{left}\n{GetTabs()}{right}{DecTab()}"; }