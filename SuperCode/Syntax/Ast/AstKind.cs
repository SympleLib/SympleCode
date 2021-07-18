namespace SuperCode
{
	public enum AstKind
	{
		Module,
		Type,

		FuncMem,

		RetStmt,
		VarStmt,
		ExprStmt,

		LitExpr,
		BinExpr,

		Count,
		Stmt = VarStmt,
		Expr = LitExpr,
	}
}
