﻿namespace SuperCode
{
	public struct Token
	{
		public static readonly Token error = new (TokenKind.Unknown, "", "", 0);

		public readonly TokenKind kind;
		public readonly string text;
		public readonly string file;
		public readonly int line;

		public Token(TokenKind kind, string txt, string path, int ln)
		{
			this.kind = kind;
			text = txt;
			file = path;
			line = ln;
		}

		public bool Is(params TokenKind[] kinds)
		{
			foreach (var kind in kinds)
				if (this.kind == kind)
					return true;
			return false;
		}

		public override string ToString() =>
			$"{file}:{line}> {kind} `{text}`";

		public static readonly string[] punctuators = {
			"+", "-", "*", "/", "%",
		};
	}

	public enum TokenKind
	{
		Unknown,

		Identifier,
		Number,

		Plus,
		Minus,
		Star,
		Slash,
		Percent,

		Count,
		Punctuator = Plus,
	}

	public static class TokenFacts
	{
		public static int BinPri(this TokenKind kind) =>
			kind switch
			{
				TokenKind.Plus or TokenKind.Minus =>
					1,
				TokenKind.Star or TokenKind.Slash or TokenKind.Percent =>
					2,

				_ =>
					0,
			};
	}
}
