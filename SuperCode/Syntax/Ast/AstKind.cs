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
