namespace SuperCode
{
	public enum NodeKind
	{
		Module,
		Field,

		DeclFuncMem,
		ImportMem,
		StructMem,
		FuncMem,
		VarMem,

		BlockStmt,
		IfStmt,
		RetStmt,
		TypedefStmt,
		VarStmt,
		WhileStmt,

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
