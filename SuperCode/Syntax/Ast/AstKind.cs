namespace SuperCode
{
	public enum AstKind
	{
		Module,
		Type,

		FuncMem,

		VarStmt,
		ExprStmt,

		LitExpr,
		BinExpr,

		Count,
		Stmt = VarStmt,
		Expr = LitExpr,
	}
}
