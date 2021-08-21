namespace SuperCode
{
	public enum NodeKind
	{
		Module,
		Field,

		DeclFuncMem,
		DeclStructMem,
		DeclVarMem,
		ImportMem,
		StructMem,
		FuncMem,
		VarMem,

		AsmStmt,
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
