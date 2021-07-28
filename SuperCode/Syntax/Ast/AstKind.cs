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

		ExprStmt,
		RetStmt,
		VarStmt,

		BinExpr,
		CallExpr,
		CastExpr,
		LitExpr,
		ParenExpr,
		PreExpr,
		TypePunExpr,

		Count,
	}
}
