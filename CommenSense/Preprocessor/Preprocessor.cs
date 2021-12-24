using System.IO;

namespace CommenSense;

// Psuedo Parser
class Preprocessor
{
	readonly string source;
	readonly string folder;
	readonly string filename;
	readonly List<Token> tokens = new List<Token>();
	readonly List<string> defines = new List<string>();

	readonly List<bool> codeYes = new List<bool>();
	readonly List<bool> ifYes = new List<bool>();

	private Lexer? lxr;

	public Preprocessor(string source, string folder, string filename)
	{
		this.source = source;
		this.folder = folder;
		this.filename = filename;
	}

	public Token[] PreProcess()
	{
		Lexer lxr = new Lexer(source);
		Token token = lxr.LexNext();
		codeYes.Add(true);

		do
		{
			if (token.kind is TokenKind.HashTag)
				RunCommand(token.text);
			else if (codeYes[^1])
				tokens.Add(token);
			token = lxr.LexNext();
		}
		while (token.kind is not TokenKind.Eof);
		tokens.Add(token);

		return tokens.ToArray();
	}

	void RunCommand(string line)
	{
		lxr = new Lexer(line);
		string name = lxr.LexNext().text;

		switch (name)
		{
		case "define":
			defines.Add(lxr.LexNext().text);
			break;
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

	bool Expr()
	{
		string x = lxr!.LexNext().text;
		if (x == "def")
		{
			return defines.Contains(lxr.LexNext().text);
		}

		return false;
	}
}