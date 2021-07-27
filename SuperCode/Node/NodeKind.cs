namespace SuperCode
{
	public enum NodeKind
	{
		Module,
		Field,

		DeclFuncMem,
		StructMem,
		FuncMem,

		RetStmt,
		VarStmt,

		AssignExpr,
		CallExpr,
		CastExpr,
		BinExpr,
		FNumExpr,
		MemExpr,
		NumExpr,
		StrExpr,
		SymExpr,
		TypePunExpr,
		UnExpr,
	}
}
