namespace SuperCode
{
	public enum AstKind
	{
		Element,
		Field,
		Module,
		Type,

		DeclFuncMem,
		DeclVarMem,
		FuncMem,
		ImplFuncMem,
		ImplVarMem,
		StmtMem,
		StructMem,
		VarMem,

		AsmStmt,
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
