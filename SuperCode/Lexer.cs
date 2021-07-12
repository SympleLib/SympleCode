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
			bool didDot = false;

			int begin = pos;
			while (char.IsDigit(current) || current == '.')
			{
				if (current == '.')
				{
					if (didDot)
						Console.Error.WriteLine("Tooo many dots for poor number to handle");
					didDot = true;
				}
				Next();
			}

			return MakeToken(TokenKind.Number, begin);
		}

		private Token Punctuator()
		{
			for (int i = 0; i < Token.punctuators.Length; i++)
			{
				string punc = Token.punctuators[i];
				if (src[pos..(pos + punc.Length)] == punc)
				{
					int begin = pos;
					pos += punc.Length;
					return MakeToken(TokenKind.Punctuator + i, begin);
				}
			}

			Next();
			return MakeToken(TokenKind.Unknown, pos - 1);
		}

		private Token MakeToken(TokenKind kind, int begin) =>
			new (kind, src[begin..pos], path, line);
	}
}
