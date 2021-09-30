namespace CommenSense;

partial class Parser
{
	readonly Lexer lxr;
	readonly List<Token> tokens = new List<Token>();
	readonly List<string> types = new List<string>();
	int pos = 0;
	Token prev => Peek(-1);
	Token current => Peek();
	Token next => Peek(1);

	public Parser(string source)
	{
		lxr = new Lexer(source);
		Token token = lxr.LexNext();
		tokens.Add(token);
		while (token.kind is not TokenKind.Eof)
		{
			token = lxr.LexNext();
			tokens.Add(token);
		}
	}

	public ModuleAst Parse()
	{
		const string name = "simple-code";
		List<StmtAst> members = new List<StmtAst>();
		while (current.kind is not TokenKind.Eof)
			members.Add(Stmt());

		return new ModuleAst(name, members.ToArray(), types.ToArray());
	}

	TypeAst Type()
	{
		string typeBase = Name();
		int ptrCount = 0;
		while (current.kind is TokenKind.Star)
		{
			ptrCount++;
			Next();
		}

		return new TypeAst(typeBase, ptrCount);
	}

	ParamAst Param()
	{
		TypeAst type = Type();
		string name = current.kind is TokenKind.Identifier ? Next().text : string.Empty;
		ExprAst defaultExpr = new ExprAst();
		if (current.kind is TokenKind.Eql)
		{
			Next();
			defaultExpr = Expr();
		}

		return new ParamAst(type, name, defaultExpr);
	}

	FieldAst Field()
	{
		const LLVMVisibility vis = LLVMDefaultVisibility;

		TypeAst type = Type();
		string name = current.kind is TokenKind.Identifier ? Next().text : string.Empty;
		ExprAst initializer = new ExprAst();
		if (current.kind is TokenKind.Eql)
		{
			Next();
			initializer = Expr();
		}

		return new FieldAst(vis, type, name, initializer);
	}

	Token Peek(int offset = 0)
	{
		int i = pos + offset;
		if (i >= tokens.Count)
			return tokens.Last();
		return tokens[i];
	}

	Token Match(TokenKind kind)
	{
		if (current.kind == kind)
			return Next();
		throw new Exception($"incorrect tokenkind: {current.kind}");
	}

	Token Next()
	{
		pos++;
		return prev;
	}

	string Name() =>
		Match(TokenKind.Identifier).text;
}