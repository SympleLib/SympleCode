using System.IO;

namespace CommenSense;

// Psuedo Parser
class Preprocessor
{
	readonly string source;
	readonly string folder;
	readonly string filename;
	readonly List<Token> tokens = new List<Token>();
	readonly Dictionary<string, Token[]> defines = new Dictionary<string, Token[]>();

	readonly List<bool> codeYes = new List<bool>();
	readonly List<bool> ifYes = new List<bool>();

	private Lexer lxr;
	private Token current;

	public Preprocessor(string source, string folder, string filename)
	{
		this.source = source;
		this.folder = folder;
		this.filename = filename;
	}

	public Token[] PreProcess()
	{
		Lexer lxr = new Lexer(source);
		current = lxr.LexNext();
		codeYes.Add(true);

		do
		{
			if (current.kind is TokenKind.HashTag)
			{
				RunCommand(current.text);
				this.lxr = lxr;
				Next();
			}
			else if (codeYes[^1])
			{
				this.lxr = lxr;
				AppendToken(tokens);
			}
			else
				Next();
		}
		while (current.kind is not TokenKind.Eof);
		tokens.Add(current);

		return tokens.ToArray();
	}

	void RunCommand(string line)
	{
		lxr = new Lexer(line);
		Next();
		string name = Next().text;

		switch (name)
		{
		case "define":
		{
			string define = Next().text;
			List<Token> tokens = new List<Token>();
			AppendTokens(tokens);
			defines.Add(define, tokens.ToArray());
			break;
		}
		case "if":
		{
			bool yes = Expr();
			codeYes.Add(yes);
			ifYes.Add(yes);
			break;
		}
		case "elif":
		{
			bool yes = Expr();
			codeYes[^1] = !ifYes[^1] && yes;
			ifYes[^1] |= yes;
			break;
		}
		case "else":
		{
			codeYes[^1] = !ifYes[^1];
			break;
		}
		case "endif":
			if (codeYes.Count <= 1 || ifYes.Count < 0)
				throw new Exception("no matching if for #endif");
			codeYes.RemoveAt(codeYes.Count - 1);
			ifYes.RemoveAt(ifYes.Count - 1);
			break;
		}
	}

	void AppendTokens(List<Token> list)
	{
		if (!codeYes[^1])
			return;
		
		do
			AppendToken(list);
		while (current.kind is not TokenKind.Eof);
	}

	void AppendToken(List<Token> list)
	{
		if (current.kind is TokenKind.Eof)
			return;
			
		if (defines.ContainsKey(current.text))
			list.AddRange(defines[current.text]);
		else
			list.Add(current);
		Next();
	}

	bool Expr()
	{
		List<Token> _ = new List<Token>();
		AppendToken(_);
		if (_[0].text is "true" or "1")
			return true;
		if (_[0].text == "def")
			return defines.ContainsKey(Next().text);

		return false;
	}

	Token Next()
	{
		Token c = current;
		current = lxr.LexNext();
		return c;
	}

	Token Match(TokenKind kind)
	{
		if (current.kind == kind)
			return Next();
		BadCode.Report(new SyntaxError($"expected {kind}", current));
		return current;
	}
}