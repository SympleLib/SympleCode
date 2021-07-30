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

		BinExpr,
		CallExpr,
		CastExpr,
		IndexExpr,
		LitExpr,
		ParenExpr,
		PreExpr,
		TypePunExpr,

		Count,
	}
}
