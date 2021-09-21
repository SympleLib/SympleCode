﻿namespace CommenSense;

partial class Parser
{
	StmtAst Stmt()
	{
		if (current.kind is TokenKind.DeclKeyword)
		{
			Next();
			TypeAst type = Type();
			string name = Match(TokenKind.Identifier).text;
			if (current.kind is TokenKind.LeftParen)
				return DeclFunc(retType: type, name);
			return DeclVar(type, name);
		}
		if (current.kind is TokenKind.Identifier && !(scope.VarExists(current.text) || scope.FuncExists(current.text)))
		{
			TypeAst type = Type();
			string name = Match(TokenKind.Identifier).text;
			if (current.kind is TokenKind.LeftParen or TokenKind.LeftBrace)
				return Func(retType: type, name);
			return Var(type, name);
		}

		return ExprStmt();
	}

	FuncAst Func(TypeAst retType, string name)
	{
		EnterScope();
		const LLVMVisibility vis = LLVMDefaultVisibility;
		bool vaArg = false;
		List<ParamAst> paramz = new List<ParamAst>();
		if (current.kind is TokenKind.LeftParen)
		{
			Next();
			while (current.kind is not TokenKind.Eof and not TokenKind.RightParen)
			{
				if (current.kind is TokenKind.DotDotDot)
				{
					Next();
					vaArg = true;
					break;
				}

				ParamAst param = Param();
				paramz.Add(param);
				scope.DefineVar(param.name);
				if (current.kind is not TokenKind.RightParen)
					Match(TokenKind.Comma);
			}

			Match(TokenKind.RightParen);
		}

		List<StmtAst> body = new List<StmtAst>();
		Match(TokenKind.LeftBrace);
		while (current.kind is not TokenKind.Eof and not TokenKind.RightBrace)
			body.Add(Stmt());
		ExitScope();
		Match(TokenKind.RightBrace);

		scope.DefineFunc(name);
		return new FuncAst(vis, retType, name, paramz.ToArray(), body.ToArray(), vaArg);
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

	DeclFuncAst DeclFunc(TypeAst retType, string name)
	{
		const LLVMVisibility vis = LLVMDefaultVisibility;
		List<ParamAst> paramz = new List<ParamAst>();
		Match(TokenKind.LeftParen);
		bool vaArg = false;
		while (current.kind is not TokenKind.Eof and not TokenKind.RightParen)
		{
			if (current.kind is TokenKind.DotDotDot)
			{
				Next();
				vaArg = true;
				break;
			}

			ParamAst param = Param();
			paramz.Add(param);
			if (current.kind is not TokenKind.RightParen)
				Match(TokenKind.Comma);
		}

		Match(TokenKind.RightParen);

		scope.DefineFunc(name);
		return new DeclFuncAst(vis, retType, name, paramz.ToArray(), vaArg);
	}

	DeclVarAst DeclVar(TypeAst type, string name)
	{
		const LLVMVisibility vis = LLVMDefaultVisibility;
		scope.DefineVar(name);
		return new DeclVarAst(vis, type, name);
	}

	ExprStmtAst ExprStmt() =>
		new ExprStmtAst(Expr());
}