namespace SuperCode
{
	public enum NodeKind
	{
		Module,
		Field,

		DeclFuncMem,
		DeclVarMem,
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

		ArrExpr,
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
