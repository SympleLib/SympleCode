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
		switch (current.kind)
		{
		case TokenKind.UsingKeyword:
			JumpTo(TokenKind.AsKeyword);
			typeNames.Add(Match(TokenKind.Identifier).text);
			MaybeEndLine();
			return;
		case TokenKind.LinkKeyword:
			
		}

		switch (current.kind)
		{
		case TokenKind.PublicKeyword:
		case TokenKind.PrivateKeyword:
		case TokenKind.ProtectedKeyword:
			Next();
			break;
		};

		if (current.kind is TokenKind.StructKeyword)
			PreStruct();
		else if (current.kind is TokenKind.ClassKeyword)
			PreClass();
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
			bool asmified = false;
			if (current.kind is TokenKind.Colon)
			{
				asmified = true;

				Next();
				Match(TokenKind.Str);
			}

			if (current.kind is TokenKind.LeftParen or TokenKind.LeftBrace or TokenKind.Arrow)
				PreFunc(name, asmified);
			else
				PreVar(name, asmified);
		}
	}

	void PreStruct()
	{
		Match(TokenKind.StructKeyword);
		typeNames.Add(Name());
		Follow(TokenKind.LeftBrace, TokenKind.RightBrace);
		MaybeEndLine();
	}

	void PreClass()
	{
		Match(TokenKind.ClassKeyword);
		typeNames.Add(Name());
		Follow(TokenKind.LeftBrace, TokenKind.RightBrace);
		MaybeEndLine();
	}

	void PreFunc(string name, bool asmified)
	{
		if (current.kind is TokenKind.LeftParen)
			Follow(TokenKind.LeftParen, TokenKind.RightParen);
		if (!asmified && current.kind is TokenKind.Colon)
		{
			Next();
			Match(TokenKind.Str);
		}

		if (current.kind is TokenKind.Annotation)
			Next();

		if (current.kind is TokenKind.Arrow)
			JumpTo(TokenKind.Semicol);
		else
			Follow(TokenKind.LeftBrace, TokenKind.RightBrace);
		MaybeEndLine();

		funcNames.Add(name);
	}

	void PreVar(string name, bool asmified)
	{
		if (!asmified && current.kind is TokenKind.Colon)
		{
			Next();
			Match(TokenKind.Str);
		}

		JumpTo(TokenKind.Semicol);
		MaybeEndLine();

		varNames.Add(name);
	}

	void PreDeclFunc(string name)
	{
		Follow(TokenKind.LeftParen, TokenKind.RightParen);
		if (current.kind is TokenKind.Colon)
		{
			Next();
			Match(TokenKind.Str);
		}

		if (current.kind is TokenKind.Annotation)
			Next();

		MaybeEndLine();

		funcNames.Add(name);
	}

	void PreDeclVar(string name)
	{
		if (current.kind is TokenKind.Colon)
		{
			Next();
			Match(TokenKind.Str);
		}

		MaybeEndLine();
		varNames.Add(name);
	}


	void JumpTo(TokenKind kind)
	{
		while (current.kind is not TokenKind.Eof && current.kind != kind)
			Next();
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