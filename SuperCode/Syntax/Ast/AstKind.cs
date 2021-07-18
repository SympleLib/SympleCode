namespace SuperCode
{
	public enum AstKind
	{
		Module,
		Param,
		Type,

		FuncMem,
		DeclFuncMem,

		RetStmt,
		VarStmt,
		ExprStmt,

		LitExpr,
		BinExpr,
		CallExpr,
		CastExpr,

		Count,
	}
}
