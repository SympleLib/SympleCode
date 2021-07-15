namespace SuperCode
{
	public enum AstKind
	{
		Unknown,
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
