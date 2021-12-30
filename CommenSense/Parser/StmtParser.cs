using System.Text;
using LLVMSharp.Interop;

namespace CommenSense;

partial class Parser
{
	StmtAst Stmt()
	{
		switch (current.kind)
		{
		case TokenKind.LeftBrace:
			return Block();
		case TokenKind.IfKeyword:
			return If();
		case TokenKind.WhileKeyword:
			return While();
		case TokenKind.ForKeyword:
			return For();
		case TokenKind.BreakKeyword:
			return Break();
		case TokenKind.BreakAllKeyword:
			return BreakAll();
		case TokenKind.RetKeyword:
			return Ret();
		}

		List<string> metadata = new List<string>();
		while (current.kind is TokenKind.Annotation)
			metadata.Add(Next().text);

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
				return DeclFunc(metadata.ToArray(), visibility, retType: type, name);
			return DeclVar(metadata.ToArray(), visibility, type, name);
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
				return Func(metadata.ToArray(), visibility, retType: type, name, asmName);
			return Var(metadata.ToArray(), visibility, type, name, asmName);
		}

		if (!illegal)
			return ExprStmt();

		throw new Exception("Illegal");
	}

	BlockStmtAst Block()
	{
		Token open = Match(TokenKind.LeftBrace);
		List<StmtAst> stmts = new List<StmtAst>();
		while (current.kind is not TokenKind.Eof and not TokenKind.RightBrace)
		{
			int start = pos;
			stmts.Add(Stmt());
			if (start == pos)
				break;
		}

		Match(TokenKind.RightBrace);

		return new BlockStmtAst(open, stmts.ToArray());
	}

	IfStmtAst If()
	{
		Token keywrd = Match(TokenKind.IfKeyword);
		Match(TokenKind.LeftParen);
		ExprAst cond = Expr();
		Match(TokenKind.RightParen);
		StmtAst then = Stmt();

		StmtAst? elze = null;
		if (current.kind is TokenKind.ElseKeyword)
		{
			Next();
			elze = Stmt();
		}

		return new IfStmtAst(keywrd, cond, then, elze);
	}

	WhileStmtAst While()
	{
		Token keywrd = Match(TokenKind.WhileKeyword);
		Match(TokenKind.LeftParen);
		ExprAst cond = Expr();
		Match(TokenKind.RightParen);
		StmtAst then = Stmt();

		return new WhileStmtAst(keywrd, cond, then);
	}

	ForStmtAst For()
	{
		Token keywrd = Match(TokenKind.ForKeyword);
		Match(TokenKind.LeftParen);
		StmtAst init = Stmt();
		ExprAst cond = Expr();
		Match(TokenKind.Semicol);
		ExprAst step = Expr();
		Match(TokenKind.RightParen);
		StmtAst then = Stmt();

		return new ForStmtAst(keywrd, init, cond, step, then);
	}

	BreakStmtAst Break()
	{
		Token keywrd = Match(TokenKind.BreakKeyword);
		int depth = 1;
		if (current.kind is not TokenKind.Semicol)
		{
			Token depthKwrd = Next();
			if (depthKwrd.kind is not TokenKind.Int)
			{
				BadCode.Report(new SyntaxError("expected positive integer above 0", depthKwrd));
				MaybeEndLine();
				return new BreakStmtAst(keywrd, depth);
			}

			depth = int.Parse(depthKwrd.text);
			if (depth <= 0)
				BadCode.Report(new SyntaxError("expected positive integer above 0", depthKwrd));
		}
		EndLine();
		return new BreakStmtAst(keywrd, depth);
	}

	BreakAllStmtAst BreakAll()
	{
		Token keywrd = Match(TokenKind.BreakAllKeyword);
		EndLine();
		return new BreakAllStmtAst(keywrd);
	}

	RetStmtAst Ret()
	{
		Token keywrd = Match(TokenKind.RetKeyword);
		ExprAst expr;
		if (current.kind is TokenKind.Semicol)
			expr = new ExprAst(keywrd);
		else
			expr = Expr();
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
			fields.Add(Field(fields));
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
			fields.Add(Field(fields));
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
		{
			funcs.Add(Func(name.text, fields.ToArray()));
		}

		Match(TokenKind.RightBrace);
		MaybeEndLine();
		var clazz = new ClassAst(visibility, name, fields.ToArray(), funcs.ToArray());
		ctnrs.Add(clazz);
		return clazz;
	}

	FuncAst Func(string className, FieldAst[] fields)
	{
		List<string> metadata = new List<string>();
		while (current.kind is TokenKind.Annotation)
			metadata.Add(Next().text);
		
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
		string asmName = $"{className}.{name.text}";

		EnterScope();
		foreach (FieldAst field in fields)
			scope.DefineVar(field.name);
		scope.DefineVar("this");
		FuncAst func = Func(metadata.ToArray(), visibility, retType, name, asmName, false);
		ExitScope();
		return func;
	}

	FuncAst Func(string[] metadata, Visibility visibility, TypeAst retType, Token name, string? asmName, bool asmNameFinal = true)
	{
		EnterScope();
		bool vaArg = false;
		List<ParamAst> paramz = new List<ParamAst>();
		Match(TokenKind.LeftParen);
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

		if (asmName is null || !asmNameFinal)
		{
			if (current.kind is TokenKind.Colon)
			{
				Next();
				asmName = Match(TokenKind.Str).text;
			}
			else if (metadata.Contains("naked"))
				asmName = name.text;
			else
			{
				StringBuilder sb = new StringBuilder("Syf$");
				sb.Append(name.text);
				foreach (ParamAst param in paramz)
				{
					sb.Append(';');
					sb.Append(param.type.name);
				}
				
				asmName = sb.ToString();
			}
		}

		// Single Expr Func
		if (current.kind is TokenKind.Arrow)
		{
			Next();
			ExprAst expr = Expr();
			MaybeEndLine();
			return new FuncAst(metadata, visibility, retType, name, asmName?? name.text, paramz.ToArray(), vaArg, new StmtAst[] { new RetStmtAst(Token.devault, expr) });
		}

		List<StmtAst> body = new List<StmtAst>();
		Match(TokenKind.LeftBrace);
		while (current.kind is not TokenKind.Eof and not TokenKind.RightBrace)
		{
			int start = pos;
			body.Add(Stmt());
			if (start == pos)
				break;
		}
		ExitScope();
		Match(TokenKind.RightBrace);

		MaybeEndLine();
		scope.DefineFunc(name.text);
		return new FuncAst(metadata, visibility, retType, name, asmName?? name.text, paramz.ToArray(), vaArg, body.ToArray());
	}

	VarAst Var(string[] metadata, Visibility visibility, TypeAst type, Token name, string? asmName)
	{
		scope.DefineVar(name.text);
		if (asmName is null)
		{
			if (current.kind is TokenKind.Colon)
			{
				Next();
				asmName = Match(TokenKind.Str).text;
			}
			else
				asmName = $"Syv${name.text}";
		}

		ExprAst initializer = new ExprAst(Token.devault);
		if (current.kind is TokenKind.Eql)
		{
			Next();
			initializer = Expr();
		}

		EndLine();
		return new VarAst(metadata, visibility, type, name, asmName, initializer);
	}

	DeclFuncAst DeclFunc(string[] metadata, Visibility visibility, TypeAst retType, Token name)
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

		string asmName;
		if (current.kind is TokenKind.Colon)
		{
			Next();
			asmName = Match(TokenKind.Str).text;
		}
		else if (metadata.Contains("naked"))
			asmName = name.text;
		else
		{
			StringBuilder sb = new StringBuilder("Syf$");
			sb.Append(name.text);
			foreach (ParamAst param in paramz)
			{
				sb.Append(';');
				sb.Append(param.type.name);
			}

			asmName = sb.ToString();
		}

		MaybeEndLine();

		return new DeclFuncAst(metadata, visibility, retType, name, asmName, paramz.ToArray(), vaArg);
	}

	DeclVarAst DeclVar(string[] metadata, Visibility visibility, TypeAst type, Token name)
	{
		scope.DefineVar(name.text);
		string asmName = $"Syv${name.text}";
		if (current.kind is TokenKind.Colon)
		{
			Next();
			name = Match(TokenKind.Str);
		}
		return new DeclVarAst(metadata, visibility, type, name, asmName);
	}

	ExprStmtAst ExprStmt()
	{
		ExprStmtAst stmt = new ExprStmtAst(Expr());
		EndLine();
		return stmt;
	}
}