namespace SuperCode
{
	public enum AstKind
	{
		Element,
		Field,
		Module,
		Type,

		DeclFuncMem,
		FuncMem,
		ImplFuncMem,
		StmtMem,
		StructMem,
		VarMem,

		BlockStmt,
		ExprStmt,
		IfStmt,
		ImportMem,
		RetStmt,
		UsingStmt,
		VarStmt,
		WhileStmt,

		ArrExpr,
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
