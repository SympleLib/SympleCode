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
		TypedefStmt,
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
