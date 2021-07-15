using System;
using System.Collections.Generic;
using System.IO;

namespace SuperCode
{
	public class Lexer
	{
		private readonly string path;
		private readonly string src;
		private int line;
		private int pos;

		private char current => pos < src.Length ? src[pos] : '\0';
		private char next => pos + 1 < src.Length ? src[pos + 1] : '\0';

		public Lexer(string path)
		{
			this.path = path;
			src = File.ReadAllText("../../../" + path);
			line = 1;
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
				if (current == '\n')
				{
					line++;
					Next();
					continue;
				}

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

				if (char.IsDigit(current) || current == '.')
				{
					tokens.Add(Number());
					continue;
				}

				tokens.Add(Punctuator());
			}

			tokens.Add(MakeToken(TokenKind.Eof, "\0"));
			return tokens.ToArray();
		}

		private Token Identifier()
		{
			bool IsIden(char c) =>
				char.IsLetterOrDigit(c) || c == '_' || c == '$';

			int begin = pos;
			while (IsIden(current) ||
				(current == '-' && IsIden(next))) // <-- This ones for you, Swerdlow
				Next();
			return Keyword(src[begin..pos]);
		}

		private Token Number()
		{
			bool didDot = false;

			int begin = pos;
			while (char.IsDigit(current) || current == '.')
			{
				if (current == '.')
				{
					if (didDot)
						throw new Exception("Tooo many dots for poor number to handle");
					didDot = true;
					if (!char.IsDigit(next))
						break;
				}
				Next();
			}

			return MakeToken(TokenKind.Num, begin);
		}

		private Token Punctuator()
		{
			for (int i = Token.puncs.Length - 1; i >= 0; i--)
			{
				string punc = Token.puncs[i];
				bool works = true;
				for (int j = 0; j < punc.Length; j++)
					if (src[pos + j] != punc[j])
					{
						works = false;
						break;
					}

				if (works)
				{
					int begin = pos;
					pos += punc.Length;
					return MakeToken(TokenFacts.firstPunc + i, begin);
				}
			}

			Next();
			return MakeToken(TokenKind.Unknown, pos - 1);
		}

		private Token Keyword(string txt)
		{
			foreach (var key in Token.keys)
				if (key.Key == txt)
					return MakeToken(key.Value, txt);
			return MakeToken(TokenKind.Iden, txt);
		}

		private Token MakeToken(TokenKind kind, int begin) =>
			new (kind, src[begin..pos], path, line);

		private Token MakeToken(TokenKind kind, string txt) =>
			new(kind, txt, path, line);
	}
}
