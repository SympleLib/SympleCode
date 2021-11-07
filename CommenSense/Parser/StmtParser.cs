using LLVMSharp.Interop;

namespace CommenSense;

partial class Parser
{
	StmtAst Stmt()
	{
		switch (current.kind)
		{
		case TokenKind.RetKeyword:
			return Ret();
		}

		bool illegal = true;
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
			illegal = false;
			break;
		};

		if (current.kind is TokenKind.StructKeyword)
			return Struct(visibility);
		if (current.kind is TokenKind.ClassKeyword)
			return Class(visibility);
		if (current.kind is TokenKind.UsingKeyword)
			return Using(visibility);
		if (current.kind is TokenKind.LinkKeyword)
			return Link(visibility);
		if (current.kind is TokenKind.DeclKeyword)
		{
			Next();
			TypeAst type = Type();
			Token name = Match(TokenKind.Identifier);
			if (current.kind is TokenKind.LeftParen)
				return DeclFunc(visibility, retType: type, name);
			return DeclVar(visibility, type, name);
		}
		if (current.kind is TokenKind.Identifier && !(scope.VarExists(current.text) || scope.FuncExists(current.text)) && IsType(current))
		{
			TypeAst type = Type();
			Token name = Match(TokenKind.Identifier);
			string? asmName = null;
			if (current.kind is TokenKind.Colon)
			{
				Next();
				asmName = Match(TokenKind.Str).text;
			}
			if (current.kind is TokenKind.LeftParen or TokenKind.LeftBrace or TokenKind.Arrow)
				return Func(visibility, retType: type, name, asmName);
			return Var(visibility, type, name, asmName);
		}

		if (!illegal)
			return ExprStmt();

		throw new Exception("Illegal");
	}

	RetStmtAst Ret()
	{
		Token keywrd = Match(TokenKind.RetKeyword);
		ExprAst expr = Expr();
		EndLine();
		return new RetStmtAst(keywrd, expr);
	}

	UsingAst Using(Visibility visibility)
	{
		Token keywrd = Match(TokenKind.UsingKeyword);
		TypeAst realType = Type();
		Match(TokenKind.AsKeyword);
		string alias = Name();
		EndLine();

		return new UsingAst(visibility, keywrd, realType, alias);
	}

	LinkAst Link(Visibility visibility)
	{
		Token keywrd = Match(TokenKind.LinkKeyword);
		string filename = Match(TokenKind.Str).text;
		EndLine();

		return new LinkAst(visibility, keywrd, filename);
	}

	StructAst Struct(Visibility visibility)
	{
		Match(TokenKind.StructKeyword);
		Token name = Match(TokenKind.Identifier);
		Match(TokenKind.LeftBrace);

		List<FieldAst> fields = new List<FieldAst>();
		while (current.kind is not TokenKind.Eof and not TokenKind.RightBrace)
		{
			int start = pos;
			fields.Add(Field());
			if (start == pos)
				break;

			if (current.kind is not TokenKind.RightBrace)
				Match(TokenKind.Comma);
		}

		Match(TokenKind.RightBrace);

		MaybeEndLine();
		var ztruct = new StructAst(visibility, name, fields.ToArray());
		ctnrs.Add(ztruct);
		return ztruct;
	}

	ClassAst Class(Visibility visibility)
	{
		Match(TokenKind.ClassKeyword);
		Token name = Match(TokenKind.Identifier);
		Match(TokenKind.LeftBrace);

		List<FieldAst> fields = new List<FieldAst>();
		while (current.kind is not TokenKind.Eof and not TokenKind.Semicol)
		{
			int start = pos;
			fields.Add(Field());
			if (start == pos)
				break;

			// maybe made function?
			if (current.kind is TokenKind.LeftParen)
			{
				BadCode.Report(new SyntaxError("func is not field", tokens[start]));
				break;
			}

			if (current.kind is not TokenKind.Semicol)
				Match(TokenKind.Comma);
		}

		EndLine();

		List<FuncAst> funcs = new List<FuncAst>();
		while (current.kind is not TokenKind.Eof and not TokenKind.RightBrace)
			funcs.Add(Func(name.text));

		Match(TokenKind.RightBrace);
		MaybeEndLine();
		var clazz = new ClassAst(visibility, name, fields.ToArray(), funcs.ToArray());
		ctnrs.Add(clazz);
		return clazz;
	}

	FuncAst Func(string className)
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

		TypeAst retType = Type();
		Token name = Match(TokenKind.Identifier);
		string asmName = $"{className}.{name}";
		if (current.kind is TokenKind.Colon)
		{
			Next();
			asmName = Match(TokenKind.Str).text;
		}

		return Func(visibility, retType, name, asmName);
	}

	FuncAst Func(Visibility visibility, TypeAst retType, Token name, string? asmName)
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

				int start = pos;
				ParamAst param = Param();
				if (start == pos)
					break;

				paramz.Add(param);
				scope.DefineVar(param.name);
				if (current.kind is not TokenKind.RightParen)
					Match(TokenKind.Comma);
			}

			Match(TokenKind.RightParen);
		}

		if (asmName is null)
		{
			asmName = name.text;
			if (current.kind is TokenKind.Colon)
			{
				Next();
				asmName = Match(TokenKind.Str).text;
			}
		}

		// Single Expr Func
		if (current.kind is TokenKind.Arrow)
		{
			Next();
			ExprAst expr = Expr();
			MaybeEndLine();
			return new FuncAst(visibility, retType, name, asmName?? name.text, paramz.ToArray(), vaArg, new StmtAst[] { new RetStmtAst(Token.devault, expr) });
		}

		List<StmtAst> body = new List<StmtAst>();
		Match(TokenKind.LeftBrace);
		while (current.kind is not TokenKind.Eof and not TokenKind.RightBrace)
			body.Add(Stmt());
		ExitScope();
		Match(TokenKind.RightBrace);

		MaybeEndLine();
		scope.DefineFunc(name.text);
		return new FuncAst(visibility, retType, name, asmName?? name.text, paramz.ToArray(), vaArg, body.ToArray());
	}

	VarAst Var(Visibility visibility, TypeAst type, Token name, string? asmName)
	{
		scope.DefineVar(name.text);
		if (asmName is null && current.kind is TokenKind.Colon)
		{
			Next();
			asmName = Match(TokenKind.Str).text;
		}
		ExprAst initializer = new ExprAst(Token.devault);
		if (current.kind is TokenKind.Eql)
		{
			Next();
			initializer = Expr();
		}

		EndLine();
		return new VarAst(visibility, type, name, asmName?? name.text, initializer);
	}

	DeclFuncAst DeclFunc(Visibility visibility, TypeAst retType, Token name)
	{
		scope.DefineFunc(name.text);
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

		string asmName = name.text;
		if (current.kind is TokenKind.Colon)
		{
			Next();
			asmName = Match(TokenKind.Str).text;
		}

		MaybeEndLine();

		return new DeclFuncAst(visibility, retType, name, asmName, paramz.ToArray(), vaArg);
	}

	DeclVarAst DeclVar(Visibility visibility, TypeAst type, Token name)
	{
		scope.DefineVar(name.text);
		string asmName = name.text;
		if (current.kind is TokenKind.Colon)
		{
			Next();
			name = Match(TokenKind.Str);
		}
		return new DeclVarAst(visibility, type, name, asmName);
	}

	ExprStmtAst ExprStmt()
	{
		ExprStmtAst stmt = new ExprStmtAst(Expr());
		EndLine();
		return stmt;
	}
}