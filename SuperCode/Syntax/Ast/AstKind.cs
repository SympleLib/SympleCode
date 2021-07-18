namespace SuperCode
{
	public enum AstKind
	{
		Module,
		Param,
		Type,

		DeclFuncMem,
		FuncMem,

		ExprStmt,
		RetStmt,
		VarStmt,

		BinExpr,
		CallExpr,
		CastExpr,
		LitExpr,
		ParenExpr,

		Count,
	}
}
