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

		Token Identifier()
		{
			int start = pos;
			while (char.IsLetterOrDigit(current))
				pos++;

			return new Token(TokenKind.Identifier, src[start..pos]);
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
