using System.IO;

namespace CommenSense;

partial class Parser
{
	// Store the names of symbols to tell what an identifier means
	readonly Dictionary<string, bool> preParse = new Dictionary<string, bool>();

	readonly Dictionary<string, bool>  varNames = new Dictionary<string, bool>();
	readonly Dictionary<string, bool> funcNames = new Dictionary<string, bool>();
	readonly Dictionary<string, bool> typeNames = new Dictionary<string, bool>()
	{
		{ "void", false },
		{ "bool", false },
		{ "byte", false },
		{ "char", false },
		{ "int", false },
		{ "long", false },
		{ "float", false },
		{ "double", false },
	};

	// Iterate through code as if parsing it, but only care about the top-level stmts
	// returns imports that also need to be pre-parsed
	public void PreParse()
	{
		MaybeEndLine();

		while (current.kind is not TokenKind.Eof)
			PreStmt();

		// Reset for the actual parser
		pos = 0;

		foreach (var import in preParse)
		{
			string filename;
			if (import.Key.Contains(':'))
				filename = import.Key;
			else
				filename = folder + '/' + import.Key;

			if (parsers.ContainsKey(filename))
				continue;

			string src = File.ReadAllText(filename);
			Parser tmpParser = new Parser(src, filename);
			tmpParser.PreParse();
			foreach (var varName in tmpParser.varNames)
				if (varName.Value)
					varNames.TryAdd(varName.Key, import.Value);
			foreach (var funcName in tmpParser.funcNames)
				if (funcName.Value)
					funcNames.TryAdd(funcName.Key, import.Value);
			foreach (var typeName in tmpParser.varNames)
				if (typeName.Value)
					typeNames.TryAdd(typeName.Key, import.Value);
		}
	}

	void PreStmt()
	{
		while (current.kind is TokenKind.Annotation)
			Next();
		
		bool isPublic = true;
		switch (current.kind)
		{
		case TokenKind.PublicKeyword:
			Next();
			break;
		case TokenKind.PrivateKeyword:
		case TokenKind.ProtectedKeyword:
			isPublic = false;
			Next();
			break;
		};

		if (current.kind is TokenKind.StructKeyword)
			PreStruct(isPublic);
		else if (current.kind is TokenKind.ClassKeyword)
			PreClass(isPublic);
		else if (current.kind is TokenKind.UsingKeyword)
		{
			JumpTo(TokenKind.AsKeyword);
			typeNames.Add(Match(TokenKind.Identifier).text, isPublic);
			MaybeEndLine();
		}
		else if (current.kind is TokenKind.LinkKeyword)
		{
			string filename = JumpTo(TokenKind.Str).text;
			preParse.Add(filename, isPublic);
			EndLine();
		}
		else if (current.kind is TokenKind.DeclKeyword)
		{
			Next();
			Type();
			string name = Name();
			if (current.kind is TokenKind.LeftParen)
				PreDeclFunc(isPublic, name);
			else
				PreDeclVar(isPublic, name);
		}
		else if (current.kind is TokenKind.Identifier && !(scope.VarExists(current.text) || scope.FuncExists(current.text)) && next.kind is TokenKind.Star or TokenKind.Identifier or TokenKind.LeftParen)
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
				PreFunc(isPublic, name, asmified);
			else
				PreVar(isPublic, name, asmified);
		}
	}

	void PreStruct(bool isPublic)
	{
		Match(TokenKind.StructKeyword);
		typeNames.Add(Name(), isPublic);
		Follow(TokenKind.LeftBrace, TokenKind.RightBrace);
		MaybeEndLine();
	}

	void PreClass(bool isPublic)
	{
		Match(TokenKind.ClassKeyword);
		typeNames.Add(Name(), isPublic);
		Follow(TokenKind.LeftBrace, TokenKind.RightBrace);
		MaybeEndLine();
	}

	void PreFunc(bool isPublic, string name, bool asmified)
	{
		if (current.kind is TokenKind.LeftParen)
			Follow(TokenKind.LeftParen, TokenKind.RightParen);
		if (!asmified && current.kind is TokenKind.Colon)
		{
			Next();
			Match(TokenKind.Str);
		}

		while (current.kind is TokenKind.Annotation)
			Next();

		if (current.kind is TokenKind.Arrow)
			JumpTo(TokenKind.Semicol);
		else
			Follow(TokenKind.LeftBrace, TokenKind.RightBrace);
		MaybeEndLine();

		funcNames.Add(name, isPublic);
	}

	void PreVar(bool isPublic, string name, bool asmified)
	{
		if (!asmified && current.kind is TokenKind.Colon)
		{
			Next();
			Match(TokenKind.Str);
		}

		JumpTo(TokenKind.Semicol);
		MaybeEndLine();

		varNames.Add(name, isPublic);
	}

	void PreDeclFunc(bool isPublic, string name)
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

		funcNames.Add(name, isPublic);
	}

	void PreDeclVar(bool isPublic, string name)
	{
		if (current.kind is TokenKind.Colon)
		{
			Next();
			Match(TokenKind.Str);
		}

		MaybeEndLine();
		varNames.Add(name, isPublic);
	}


	Token JumpTo(TokenKind kind)
	{
		while (current.kind is not TokenKind.Eof && current.kind != kind)
			Next();
		return Match(kind);
	}

	Token Follow(TokenKind open, TokenKind close)
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

		return Match(close);
	}
}