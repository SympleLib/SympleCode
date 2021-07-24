using System;
using System.Collections.Generic;
using System.Diagnostics.CodeAnalysis;

namespace SuperCode
{
	public enum TokenKind
	{
		Unknown,
		LongComment,
		Eof,

		Iden,
		Str,
		Num,

		Plus,
		Dash,
		Star,
		Slash,
		Percent,

		SlashDash,
		PercentDash,

		At,

		LeftBrace,
		RightBrace,
		LeftParen,
		RightParen,
		LeftBracket,
		RightBracket,

		Comma,
		Semicol,

		Eql,
		Arrow,

		DeclKey,

		RetKey,

		Count,
	}

	public partial struct Token
	{
		public readonly TokenKind kind;
		public readonly string text;
		public readonly string file;
		public readonly int line, col;
		public readonly int pos;

		public Token(TokenKind kind, string txt, string path, int ln, int col, int pos)
		{
			this.kind = kind;
			this.text = txt;
			this.file = path;
			this.line = ln;
			this.col = col;
			this.pos = pos;
		}

		public static bool operator ==(Token x, Token y) =>
			x.Equals(y);
		public static bool operator !=(Token x, Token y) =>
			!x.Equals(y);

		public override bool Equals([NotNullWhen(true)] object obj) =>
			GetHashCode() == obj.GetHashCode();

		public override int GetHashCode() =>
			HashCode.Combine(kind, text, file, line, col, pos);

		public bool Is(params TokenKind[] kinds)
		{
			foreach (var kind in kinds)
				if (this.kind == kind)
					return true;
			return false;
		}

		public override string ToString() =>
			$"'{file}' {line}:{col}> {kind} `{(Is(TokenKind.Str) ? text[1..^1] : text)}`";

		public static readonly string[] puncs = {
			"+", "-", "*", "/", "%",
			"/-", "%-",
			"@",
			"{", "}", "(", ")", "[", "]",
			",", ";",
			"=", "->",
		};

		public static readonly Dictionary<string, TokenKind> keys = new ()
		{
			{ "decl", TokenKind.DeclKey },

			{ "ret", TokenKind.RetKey },
		};
	}
}
