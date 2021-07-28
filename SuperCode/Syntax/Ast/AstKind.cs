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
		IfStmt,
		RetStmt,
		UsingStmt,
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
