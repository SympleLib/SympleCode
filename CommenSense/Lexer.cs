namespace CommenSense;

partial class Parser
{
	class Lexer
	{
		const TokenKind punctuatorStart = TokenKind.Plus;
		static readonly string[] punctuators = {
			"+",
			"-",
			"*",
			"/",
			"%",
			"/-",
			"%-",

			"&",

			"=",
			"...",

			"{",
			"}",
			"[",
			"]",

			",",
		};

		const TokenKind keywordStart = TokenKind.TrueKeyword;
		static readonly string[] keywords = {
			"true",
			"false",

			"decl",
		};

		readonly string src;
		int pos;
		char current => Peek();

		public Lexer(string source) =>
			src = source;

		public Token LexNext()
		{
			while (char.IsWhiteSpace(current))
				pos++;

			if (current is '\0')
				return new Token(TokenKind.Eof, string.Empty);

			if (char.IsDigit(current))
				return Num();
			if (char.IsLetter(current))
				return Identifier();
			if (current is '\'')
				return Str();
			return Punctuator();
		}

		Token Num()
		{
			int start = pos;
			TokenKind kind = TokenKind.Int;
			while (char.IsDigit(current) || current is '.')
			{
				if (current is '.')
				{
					if (kind == TokenKind.Int)
						kind = TokenKind.Float;
					else
						throw new Exception("too many dots");
				}

				pos++;
			}

			return new Token(kind, src[start..pos]);
		}

		Token Str()
		{
			Next();
			int start = pos;
			while (current is not '\'')
				Next();
			Next();
			return new Token(TokenKind.Str, src[start..(pos-1)]);
		}

		Token Identifier()
		{
			int start = pos;
			while (char.IsLetterOrDigit(current))
				pos++;

			string text = src[start..pos];
			for (int i = 0; i < keywords.Length; i++)
				if (keywords[i] == text)
					return new Token(keywordStart + i, text);

			return new Token(TokenKind.Identifier, text);
		}

		Token Punctuator()
		{
			for (int i = punctuators.Length - 1; i >= 0; i--)
				if (src[pos..].StartsWith(punctuators[i]))
					return new Token(punctuatorStart + i, src[pos..(pos += punctuators[i].Length)]);

			return new Token(TokenKind.Unknown, src[pos..++pos]);
		}

		char Next()
		{
			char c = current;
			if (pos < src.Length)
				pos++;
			return c;
		}

		char Peek(int offset = 0)
		{
			int i = pos + offset;
			if (i < src.Length)
				return src[i];
			return '\0';
		}
	}
}
