﻿using System;
using System.Collections.Generic;
using System.IO;
using System.Text;

namespace SuperCode
{
	public class Lexer
	{
		public readonly string path;
		public readonly string src;
		private int line, col;
		private int pos;

		private char current => pos < src.Length ? src[pos] : '\0';
		private char next => pos + 1 < src.Length ? src[pos + 1] : '\0';

		public Lexer(string path)
		{
			this.path = path;
			src = File.ReadAllText(path);
			line = 1;
			pos = 0;
		}

		public char Next()
		{
			char c = current;
			col++;
			pos++;
			return c;
		}

		public Token[] Lex()
		{
			var tokens = new List<Token>();
			while (pos < src.Length)
			{
				if (CheckNewLine())
					continue;

				if (current == '\'' || (next == '\'' && current is 's' or 'u' or 'w'))
				{
					tokens.Add(String());
					continue;
				}

				if (char.IsWhiteSpace(current))
				{
					Next();
					continue;
				}

				if (char.IsLetter(current) || current is '_')
				{
					tokens.Add(Identifier());
					continue;
				}

				if (current == '/' && next == '*')
				{
					tokens.Add(LongComment());
					continue;
				}

				if (current == '/' && next == '/')
				{
					tokens.Add(LineComment());
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

		private bool CheckNewLine()
		{
			if (current == '\n')
			{
				col = 0;
				line++;
				Next();
				return true;
			}
			return false;
		}

		private Token Identifier()
		{
			bool IsIden(char c) =>
				char.IsLetterOrDigit(c) || c is '$';

			int begin = pos;
			while (IsIden(current) ||
				(current is '-' && IsIden(next))) // <-- This ones for you, Swerdlow
				Next();
			if (current is '_' && IsIden(next))
				throw new InvalidOperationException("Sorry, but get your snake breath outta here");
			return Keyword(src[begin..pos]);
		}

		private Token String()
		{
			var sb = new StringBuilder();

			int begin = pos;
			if (current is 's' or 'u' or 'w')
				sb.Append(Next());
			sb.Append(Next());
			while (current != '\'')
			{
				if (current == 0)
					throw new InvalidOperationException("Finsh yer sentanze");
				if (!CheckNewLine())
				{
					if (current == '\\')
					{
						Next();
						switch (Next())
						{
						case '0':
							sb.Append('\0');
							break;
						case '\a':
							sb.Append('\a');
							break;
						case 'b':
							sb.Append('\b');
							break;
						case 'f':
							sb.Append('\f');
							break;
						case 'n':
							sb.Append('\n');
							break;
						case 'r':
							sb.Append('\r');
							break;
						case 't':
							sb.Append('\t');
							break;
						case 'v':
							sb.Append('\v');
							break;
						case '\\':
							sb.Append('\\');
							break;
						case '\'':
							sb.Append('\'');
							break;

						default:
							throw new InvalidOperationException("Unrecognized escape sequence");
						}
					}
					else
						sb.Append(Next());
				}
			}
			sb.Append(Next());
			return MakeToken(TokenKind.Str, sb.ToString());
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
						throw new InvalidOperationException("Tooo many dots for poor number to handle");
					didDot = true;
					if (!char.IsDigit(next) && char.IsLetter(next))
						return Punctuator();
				}
				Next();
			}

			if (current is 'f' or 'F')
				Next();

			return MakeToken(TokenKind.Num, begin);
		}

		private Token LongComment()
		{
			int begin = pos;
			Next();
			Next();
			while (!(current == '*' && next == '/'))
				if (!CheckNewLine())
					Next();
			Next();
			Next();

			return MakeToken(TokenKind.LongComment, begin);
		}

		private Token LineComment()
		{
			int begin = pos;
			Next();
			Next();
			while (!CheckNewLine())
				Next();

			return MakeToken(TokenKind.LineComment, begin);
		}

		private Token Punctuator()
		{
			for (int i = Token.puncs.Length - 1; i >= 0; i--)
			{
				string punc = Token.puncs[i];
				for (int j = 0; j < punc.Length; j++)
					if (src[pos + j] != punc[j])
						goto NextOne;

				int begin = pos;
				pos += punc.Length;
				return MakeToken(TokenFacts.firstPunc + i, begin);
			NextOne:
				continue;
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
			new(kind, src[begin..pos], path, line, col, begin);

		private Token MakeToken(TokenKind kind, string txt) =>
			new(kind, txt, path, line, col, pos - txt.Length);
	}
}
