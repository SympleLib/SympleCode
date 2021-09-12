namespace CommenSense;

partial record ModuleAst
{ public override string ToString() => $"module `{name}`: [\n{string.Join<StmtAst>(",\n", members)}\n]"; }


partial record TypeAst
{ public override string ToString() => $"{typeBase}{new string('*', ptrCount)}"; }

partial record ParamAst
{ public override string ToString() => $"[param] {type} {name} = {defaultExpr}"; }

partial record FieldAst
{ public override string ToString() => $"[field] {vis} {type} {name} = {initializer}"; }


partial record ExprStmtAst
{ public override string ToString() => expr.ToString(); }


partial record IntLiteralExprAst
{ public override string ToString() => $"{{ {value} }}"; }

partial record StrLiteralExprAst
{ public override string ToString() => $"{{ `{value}` }}"; }

partial record FloatLiteralExprAst
{ public override string ToString() => $"{{ {value} }}"; }


partial record UnExprAst
{ public override string ToString() => $"{{ {op} {operand} }}"; }

partial record BiExprAst
{ public override string ToString() => $"{{ {left} {op} {right} }}"; }