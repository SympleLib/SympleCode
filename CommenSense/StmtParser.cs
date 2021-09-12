namespace CommenSense;

partial class Parser
{
	StmtAst Stmt()
	{
		if (current.kind is TokenKind.Identifier)
			return Func();
		return ExprStmt();
	}

	FuncAst Func()
	{
		const LLVMVisibility vis = LLVMDefaultVisibility;
		TypeAst retType = Type();
		string name = Match(TokenKind.Identifier).text;
		ParamAst[] paramz = Array.Empty<ParamAst>();
		
		List<StmtAst> body = new List<StmtAst>();
		Match(TokenKind.LeftBrace);
		while (current.kind is not TokenKind.Eof and not TokenKind.RightBrace)
			body.Add(Stmt());
		Match(TokenKind.RightBrace);

		return new FuncAst(vis, retType, name, paramz, body.ToArray());
	}

	ExprStmtAst ExprStmt() =>
		new ExprStmtAst(Expr());
}