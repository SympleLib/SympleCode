namespace SuperCode
{
	public enum NodeKind
	{
		Module,
		Param,

		DeclFuncMem,
		FuncMem,

		RetStmt,
		VarStmt,

		CallExpr,
		CastExpr,
		BinExpr,
		FNumExpr,
		NumExpr,
		SymExpr,
	}
}
