namespace SuperCode
{
	public enum AstKind
	{
		Unknown,

		Module,

		VarStmt,
		ExprStmt,

		LitExpr,
		BinExpr,

		Count,
		Stmt = VarStmt,
		Expr = LitExpr,
	}
}
