namespace CommenSense;

partial class Parser
{
	readonly Lexer lxr;
	Token current = new Token(TokenKind.Unknown, string.Empty);

	public Parser(string source) =>
		lxr = new Lexer(source);

	public ModuleAst Parse()
	{
		Next();
		const string name = "simple-code";
		List<StmtAst> members = new List<StmtAst>();
		while (current.kind is not TokenKind.Eof)
			members.Add(Stmt());

		return new ModuleAst(name, members.ToArray());
	}

	TypeAst Type()
	{
		string typeBase = Match(TokenKind.Identifier).text;
		int ptrCount = 0;
		while (current.kind is TokenKind.Star)
		{
			ptrCount++;
			Next();
		}

		return new TypeAst(typeBase, ptrCount);
	}

	Token Match(TokenKind kind)
	{
		if (current.kind == kind)
			return Next();
		throw new Exception($"incorrect tokenkind: {current.kind}");
	}

	Token Next()
	{
		Token tmp = current;
		current = lxr.LexNext();
		return tmp;
	}
}