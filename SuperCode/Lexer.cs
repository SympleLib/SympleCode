using System.Collections.Generic;

namespace SuperCode
{
	public class Lexer
	{
		private string src;
		private int line;
		private int pos;

		private char current { get => pos < src.Length ? src[pos] : '\0'; }
		private char next { get => pos + 1 < src.Length ? src[pos + 1] : '\0'; }

		public Lexer(string src)
		{
			this.src = src;
			line = 0;
			pos = 0;
		}

		public char Next()
		{
			char c = current;
			pos++;
			return c;
		}

		public Token[] Lex()
		{
			var tokens = new List<Token>();
			while (pos < src.Length)
			{
				if (char.IsWhiteSpace(current))
				{
					Next();
					continue;
				}

				if (char.IsLetter(current))
				{
					tokens.Add(Identifier());
					continue;
				}

				if (char.IsDigit(current))
				{
					tokens.Add(Number());
					continue;
				}
			}

			return tokens.ToArray();
		}

		private Token Identifier()
		{
			int begin = pos;
			while (char.IsLetterOrDigit(current) || current == '_' || current == '$' ||
				current == '-') // <-- This ones for you, Swerdlow
				Next();
			return MakeToken(TokenKind.Identifier, begin);
		}

		private Token Number()
		{
			int begin = pos;
			while (char.IsDigit(current)) // <-- This ones for you, Swerdlow
				Next();
			return MakeToken(TokenKind.Number, begin);
		}

		private Token MakeToken(TokenKind kind, int begin) =>
			new Token { kind = kind, text = src[begin..pos] };
	}
}
