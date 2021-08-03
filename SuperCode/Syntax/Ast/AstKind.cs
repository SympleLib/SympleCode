namespace SuperCode
{
	public enum AstKind
	{
		Field,
		Module,
		Type,

		DeclFuncMem,
		FuncMem,
		StmtMem,
		StructMem,
		VarMem,

		BlockStmt,
		ExprStmt,
		IfStmt,
		RetStmt,
		UsingStmt,
		VarStmt,
		WhileStmt,

		BinExpr,
		CallExpr,
		CastExpr,
		IndexExpr,
		LitExpr,
		NullExpr,
		ParenExpr,
		PreExpr,
		TypePunExpr,

		Count,
	}
}
