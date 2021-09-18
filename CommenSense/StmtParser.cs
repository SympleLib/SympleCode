namespace CommenSense;

partial class Parser
{
	StmtAst Stmt()
	{
		if (current.kind is TokenKind.Identifier && !scope.VarExists(current.text))
		{
			TypeAst type = Type();
			string name = Match(TokenKind.Identifier).text;
			if (current.kind is TokenKind.LeftBrace or TokenKind.LeftBracket)
				return Func(retType: type, name);
			return Var(type, name);
		}
		return ExprStmt();
	}

	FuncAst Func(TypeAst retType, string name)
	{
		EnterScope();
		const LLVMVisibility vis = LLVMDefaultVisibility;
		List<ParamAst> paramz = new List<ParamAst>();
		if (current.kind is TokenKind.LeftBracket)
		{
			Next();
			while (current.kind is not TokenKind.Eof and not TokenKind.RightBracket)
			{
				ParamAst param = Param();
				paramz.Add(param);
				scope.DefineVar(param.name);
				if (current.kind is not TokenKind.RightBracket)
					Match(TokenKind.Comma);
			}

			Match(TokenKind.RightBracket);
		}
		
		List<StmtAst> body = new List<StmtAst>();
		Match(TokenKind.LeftBrace);
		while (current.kind is not TokenKind.Eof and not TokenKind.RightBrace)
			body.Add(Stmt());
		ExitScope();
		Match(TokenKind.RightBrace);

		return new FuncAst(vis, retType, name, paramz.ToArray(), body.ToArray());
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

		scope.DefineVar(name);
		return new VarAst(vis, type, name, initializer);
	}

	ExprStmtAst ExprStmt() =>
		new ExprStmtAst(Expr());
}