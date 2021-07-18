namespace SuperCode
{
	public enum NodeKind
	{
		Module,
		Param,

		FuncMem,
		DeclFuncMem,

		RetStmt,
		VarStmt,

		NumExpr,
		FNumExpr,
		BinExpr,
		CallExpr,
		CastExpr,
		SymExpr,
	}
}
