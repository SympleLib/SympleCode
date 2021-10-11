using LLVMSharp.Interop;

namespace CommenSense;

partial class Parser
{
	StmtAst Stmt()
	{
		if (current.kind is TokenKind.RetKeyword)
			return Ret();

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
			string? asmName = null;
			if (current.kind is TokenKind.Colon)
			{
				Next();
				name = Match(TokenKind.Str).text;
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
		Match(TokenKind.RetKeyword);
		ExprAst expr = Expr();
		EndLine();
		return new RetStmtAst(expr);
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

	FuncAst Func(Visibility visibility, TypeAst retType, string name, string? asmName)
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

		if (asmName is null)
		{
			if (current.kind is TokenKind.Colon)
			{
				Next();
				asmName = Match(TokenKind.Str).text;
			}
		}
		else
			asmName = name;

		CallConv conv = LLVMCCallConv;
		if (current.kind is TokenKind.Annotation)
		{
			string decl = Next().text;
			switch (decl)
			{
			case "cdecl":
				break;
			case "stdcall":
				conv = LLVMX86StdcallCallConv;
				break;

			default:
				throw new Exception("Invalid calling convention");
			}
		}

		// Single Expr Func
		if (current.kind is TokenKind.Arrow)
		{
			Next();
			ExprAst expr = Expr();
			MaybeEndLine();
			return new FuncAst(visibility, retType, name, asmName ?? name, paramz.ToArray(), vaArg, conv, new StmtAst[] { new RetStmtAst(expr) });
		}

		List<StmtAst> body = new List<StmtAst>();
		Match(TokenKind.LeftBrace);
		while (current.kind is not TokenKind.Eof and not TokenKind.RightBrace)
			body.Add(Stmt());
		ExitScope();
		Match(TokenKind.RightBrace);

		MaybeEndLine();
		scope.DefineFunc(name);
		return new FuncAst(visibility, retType, name, asmName?? name, paramz.ToArray(), vaArg, conv, body.ToArray());
	}

	VarAst Var(Visibility visibility, TypeAst type, string name, string? asmName)
	{
		scope.DefineVar(name);
		if (asmName is null && current.kind is TokenKind.Colon)
		{
			Next();
			asmName = Match(TokenKind.Str).text;
		}
		ExprAst initializer = new ExprAst();
		if (current.kind is TokenKind.Eql)
		{
			Next();
			initializer = Expr();
		}

		EndLine();
		return new VarAst(visibility, type, name, asmName?? name, initializer);
	}

	DeclFuncAst DeclFunc(Visibility visibility, TypeAst retType, string name)
	{
		scope.DefineFunc(name);
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

		string asmName = name;
		if (current.kind is TokenKind.Colon)
		{
			Next();
			asmName = Match(TokenKind.Str).text;
		}

		CallConv conv = LLVMCCallConv;
		if (current.kind is TokenKind.Annotation)
		{
			string decl = Next().text;
			switch (decl)
			{
			case "cdecl":
				break;
			case "stdcall":
				conv = LLVMX86StdcallCallConv;
				break;

			default:
				throw new Exception("Invalid calling convention");
			}
		}

		MaybeEndLine();

		return new DeclFuncAst(visibility, retType, name, asmName, paramz.ToArray(), vaArg, conv);
	}

	DeclVarAst DeclVar(Visibility visibility, TypeAst type, string name)
	{
		scope.DefineVar(name);
		string asmName = name;
		if (current.kind is TokenKind.Colon)
		{
			Next();
			name = Match(TokenKind.Str).text;
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