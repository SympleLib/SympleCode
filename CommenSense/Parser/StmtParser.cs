using LLVMSharp.Interop;

namespace CommenSense;

partial class Parser
{
	StmtAst Stmt()
	{
		Visibility visibility;
		switch (current.kind)
		{
		case TokenKind.PublicKeyword:
			Next();
			visibility = LLVMDefaultVisibility;
			break;
		case TokenKind.PrivateKeyword:
			Next();
			visibility = LLVMHiddenVisibility;
			break;
		case TokenKind.ProtectedKeyword:
			Next();
			visibility = LLVMProtectedVisibility;
			break;
		default:
			visibility = LLVMDefaultVisibility;
			break;
		};

		if (current.kind is TokenKind.StructKeyword)
			return Struct(visibility);
		if (current.kind is TokenKind.DeclKeyword)
		{
			Next();
			TypeAst type = Type();
			string name = Name();
			if (current.kind is TokenKind.LeftParen)
				return DeclFunc(visibility, retType: type, name);
			return DeclVar(visibility, type, name);
		}
		if (current.kind is TokenKind.Identifier && !(scope.VarExists(current.text) || scope.FuncExists(current.text)) && IsType(current))
		{
			TypeAst type = Type();
			string name = Name();
			if (current.kind is TokenKind.LeftParen or TokenKind.LeftBrace)
				return Func(visibility, retType: type, name);
			return Var(visibility, type, name);
		}

		return ExprStmt();
	}

	StructAst Struct(Visibility visibility)
	{
		Match(TokenKind.StructKeyword);
		string name = Name();
		Match(TokenKind.LeftBrace);

		List<FieldAst> fields = new List<FieldAst>();
		while (current.kind is not TokenKind.Eof and not TokenKind.RightBrace)
		{
			fields.Add(Field());
			if (current.kind is not TokenKind.RightBrace)
				Match(TokenKind.Comma);
		}

		Match(TokenKind.RightBrace);

		MaybeEndLine();
		var ztruct = new StructAst(visibility, name, fields.ToArray());
		structs.Add(ztruct);
		return ztruct;
	}

	FuncAst Func(Visibility visibility, TypeAst retType, string name)
	{
		EnterScope();
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

		MaybeEndLine();
		scope.DefineFunc(name);
		return new FuncAst(visibility, retType, name, paramz.ToArray(), body.ToArray(), vaArg);
	}

	VarAst Var(Visibility visibility, TypeAst type, string name)
	{
		ExprAst initializer = new ExprAst();
		if (current.kind is TokenKind.Eql)
		{
			Next();
			initializer = Expr();
		}

		EndLine();
		scope.DefineVar(name);
		return new VarAst(visibility, type, name, initializer);
	}

	DeclFuncAst DeclFunc(Visibility visibility, TypeAst retType, string name)
	{
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
		EndLine();

		scope.DefineFunc(name);
		return new DeclFuncAst(visibility, retType, name, paramz.ToArray(), vaArg);
	}

	DeclVarAst DeclVar(Visibility visibility, TypeAst type, string name)
	{
		scope.DefineVar(name);
		return new DeclVarAst(visibility, type, name);
	}

	ExprStmtAst ExprStmt()
	{
		ExprStmtAst stmt = new ExprStmtAst(Expr());
		EndLine();
		return stmt;
	}
}