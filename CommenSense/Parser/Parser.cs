using System.IO;

namespace CommenSense;

partial class Parser
{
	readonly string folder;
	readonly string filename;
	readonly Token[] tokens;
	readonly List<Container> ctnrs = new List<Container>();
	int pos = 0;
	Token prev => Peek(-1);
	Token current => Peek();
	Token next => Peek(1);

	public Parser(string source, string filename)
	{
		scope = new Scope(this);
		this.filename = filename;
		folder = Path.GetFileName(Path.GetDirectoryName(this.filename)!);
		parsers.Add(filename, this);

		Preprocessor preprocessor = new Preprocessor(source, folder, this.filename);
		tokens = preprocessor.PreProcess();

		foreach (Token token in tokens)
			if (token.kind is TokenKind.Unknown)
				BadCode.Report(new SyntaxError("unrecognized token", token));
			else if (token.kind is TokenKind.HashTag)
				throw new Exception("preprocessor directive after preprocessing!!");
	}

	public ModuleAst Parse()
	{
		MaybeEndLine();

		const string name = "simple-code";
		List<StmtAst> members = new List<StmtAst>();
		while (current.kind is not TokenKind.Eof)
		{
			int start = pos;
			members.Add(Stmt());
			if (start == pos)
				break;
		}

		return new ModuleAst(name, members.ToArray(), ctnrs.ToArray());
	}

	bool IsType(Token token)
	{
		if (token.kind is not TokenKind.Identifier)
			return false;

		return typeNames.ContainsKey(token.text);
	}

	TypeAst Type()
	{
		Token typeBase = Match(TokenKind.Identifier);
		int ptrCount = 0;
		while (current.kind is TokenKind.Star)
		{
			ptrCount++;
			Next();
		}
		
		TypeAst baseType = new BaseTypeAst(typeBase, ptrCount);

		if (current.kind == TokenKind.LeftParen)
		{
			Token open = Next();

			bool vaArg = false;
			List<TypeAst> paramTypes = new List<TypeAst>();
			while (current.kind is not TokenKind.Eof and not TokenKind.RightParen)
			{
				if (current.kind is TokenKind.DotDotDot)
				{
					vaArg = true;
					break;
				}

				paramTypes.Add(Type());

				if (current.kind is not TokenKind.RightParen)
					Match(TokenKind.Comma);
			}

			Match(TokenKind.RightParen);

			ptrCount = 0;
			while (current.kind is TokenKind.Star)
			{
				ptrCount++;
				Next();
			}
			return new FuncTypeAst(baseType, open, paramTypes.ToArray(), vaArg, ptrCount);
		}

		return baseType;
	}

	ParamAst Param()
	{
		TypeAst type = Type();
		Token token;
		string name;
		if (current.kind is TokenKind.Identifier)
		{
			token = Next();
			name = token.text;
		}
		else
		{
			token = type.token;
			name = string.Empty;
		}

			ExprAst defaultExpr = new ExprAst(Token.devault);
		if (current.kind is TokenKind.Eql)
		{
			Next();
			defaultExpr = Expr();
		}

		return new ParamAst(type, token, name, defaultExpr);
	}

	FieldAst Field()
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

		TypeAst type = Type();
		Token name = Match(TokenKind.Identifier);
		ExprAst initializer = new ExprAst(Token.devault);
		if (current.kind is TokenKind.Eql)
		{
			Next();
			initializer = Expr();
		}

		return new FieldAst(visibility, type, name, initializer);
	}

	Token Peek(int offset = 0)
	{
		int i = pos + offset;
		if (i >= tokens.Length)
			return tokens.Last();
		return tokens[i];
	}

	Token Match(TokenKind kind)
	{
		if (current.kind == kind)
			return Next();
		BadCode.Report(new SyntaxError($"expected {kind}", current));
		return current;
	}

	Token Next()
	{
		pos++;
		return prev;
	}

	void EndLine()
	{
		Match(TokenKind.Semicol);
		MaybeEndLine();
	}

	void MaybeEndLine()
	{
		while (current.kind is TokenKind.Semicol)
			Next();
	}

	string Name() =>
		Match(TokenKind.Identifier).text;
}