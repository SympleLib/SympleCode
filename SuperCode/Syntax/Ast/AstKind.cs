namespace SuperCode
{
	public enum AstKind
	{
		Module,

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
