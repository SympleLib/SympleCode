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

	readonly Stack<bool> codeYes = new Stack<bool>();

	public Preprocessor(string source, string folder, string filename)
	{
		this.source = source;
		this.folder = folder;
		this.filename = filename;
	}

	public Token[] PreProcess()
	{
		Lexer lxr = new Lexer(this.source);
		Token token = lxr.LexNext();
		codeYes.Push(true);

		do
		{
			if (token.kind is TokenKind.HashTag)
				RunCommand(token.text);
			else if (codeYes.First())
				tokens.Add(token);
			token = lxr.LexNext();
		}
		while (token.kind is not TokenKind.Eof);
		tokens.Add(token);

		return tokens.ToArray();
	}

	void RunCommand(string line)
	{
		Lexer lxr = new Lexer(line);
		string name = lxr.LexNext().text;

		switch (name)
		{
		case "define":
			defines.Add(lxr.LexNext().text);
			break;

		case "ifdef":
			codeYes.Push(defines.Contains(lxr.LexNext().text));
			break;
		case "endif":
			if (codeYes.Count <= 1)
				throw new Exception("no matching if for #endif");
			codeYes.Pop();
			break;
		}
	}
}