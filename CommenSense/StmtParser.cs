namespace CommenSense;

partial class Parser
{
	StmtAst Stmt()
	{
		if (current.kind is TokenKind.Identifier && next.kind is TokenKind.Identifier)
		{
			TypeAst type = Type();
			string name = Match(TokenKind.Identifier).text;
			if (current.kind is TokenKind.LeftBrace)
				return Func(retType: type, name);
			return Var(type, name);
		}
		return ExprStmt();
	}

	FuncAst Func(TypeAst retType, string name)
	{
		const LLVMVisibility vis = LLVMDefaultVisibility;
		ParamAst[] paramz = Array.Empty<ParamAst>();
		
		List<StmtAst> body = new List<StmtAst>();
		Match(TokenKind.LeftBrace);
		while (current.kind is not TokenKind.Eof and not TokenKind.RightBrace)
			body.Add(Stmt());
		Match(TokenKind.RightBrace);

		return new FuncAst(vis, retType, name, paramz, body.ToArray());
	}

	VarAst Var(TypeAst type, string name)
	{
		const LLVMVisibility vis = LLVMDefaultVisibility;
		ExprAst initializer = new ExprAst();
		if (current.kind is TokenKind.Eql)
		{
			Next();
			initializer = Expr();
		}

		return new VarAst(vis, type, name, initializer);
	}

	ExprStmtAst ExprStmt() =>
		new ExprStmtAst(Expr());
}