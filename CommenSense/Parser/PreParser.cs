namespace CommenSense;

partial class Parser
{
	// Store the names of symbols to tell what an identifier means
	readonly List<string> varNames = new List<string>();
	readonly List<string> funcNames = new List<string>();
	readonly List<string> typeNames = new List<string>()
	{
		"void",
		"bool",
		"char",
		"int",
		"long",
		"float",
		"double",
	};

	// Iterate through code as if parsing it, but only care about the top-level stmts
	void PreParse()
	{
		while (current.kind is not TokenKind.Eof)
			PreStmt();

		// Reset for the actual parser
		pos = 0;
	}

	void PreStmt()
	{
		if (current.kind is TokenKind.StructKeyword)
			PreStruct();
		else if (current.kind is TokenKind.DeclKeyword)
		{
			Next();
			Type();
			string name = Name();
			if (current.kind is TokenKind.LeftParen)
				PreDeclFunc(name);
			else
				PreDeclVar(name);
		}
		else if (current.kind is TokenKind.Identifier && !(scope.VarExists(current.text) || scope.FuncExists(current.text)) && next.kind is TokenKind.Star or TokenKind.Identifier)
		{
			Type();
			string name = Name();
			if (current.kind is TokenKind.LeftParen or TokenKind.LeftBrace)
				PreFunc(name);
			else
				PreVar(name);
		}
	}

	void PreStruct()
	{
		Match(TokenKind.StructKeyword);
		MaybeEndLine();
		typeNames.Add(Name());
		Follow(TokenKind.LeftBrace, TokenKind.RightBrace);
	}

	void PreFunc(string name)
	{
		if (current.kind is TokenKind.LeftParen)
			Follow(TokenKind.LeftParen, TokenKind.RightParen);
		Follow(TokenKind.LeftBrace, TokenKind.RightBrace);
		MaybeEndLine();

		funcNames.Add(name);
	}

	void PreVar(string name)
	{
		JumpTo(TokenKind.Semicol);
		MaybeEndLine();

		varNames.Add(name);
	}

	void PreDeclFunc(string name)
	{
		Follow(TokenKind.LeftParen, TokenKind.RightParen);
		EndLine();

		funcNames.Add(name);
	}

	void PreDeclVar(string name)
	{
		MaybeEndLine();
		varNames.Add(name);
	}


	void JumpTo(TokenKind kind)
	{
		while (current.kind is not TokenKind.Eof && current.kind != kind) {}
		Match(kind);
	}

	void Follow(TokenKind open, TokenKind close)
	{
		int depth = 0;
		Match(open);
		while (current.kind is not TokenKind.Eof)
		{
			if (current.kind == open)
				depth++;
			else if (current.kind == close)
				if (depth-- == 0)
					break;
			Next();
		}

		Match(close);
	}
}