namespace SuperCode
{
	public enum AstKind
	{
		Unknown,

		VarStmt,
		ExprStmt,

		LitExpr,
		BinExpr,

		Count,
		Stmt = VarStmt,
		Expr = LitExpr,
	}
}
