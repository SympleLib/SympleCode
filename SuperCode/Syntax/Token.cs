using System;
using System.Collections.Generic;
using System.Diagnostics.CodeAnalysis;

namespace SuperCode
{
	public enum TokenKind
	{
		Unknown,
		LineComment,
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

		At,

		LeftBrace,
		RightBrace,
		LeftParen,
		RightParen,
		LeftBracket,
		RightBracket,

		Dot,
		Comma,
		Semicol,

		Eql,
		Arrow,

		EqlEql,
		DotDotDot,

		DeclKey,
		UsingKey,
		StructKey,

		NullKey,
		RetKey,
		RefKey,
		AsKey,

		IfKey,
		ElseKey,

		Count,
	}

	public partial struct Token
	{
		public readonly TokenKind kind;
		public readonly string text;
		public readonly string rawText;
		public readonly string file;
		public readonly int line, col;
		public readonly int pos;

		public Token(TokenKind kind, string txt, string rawText, string path, int ln, int col, int pos)
		{
			this.kind = kind;
			this.text = txt;
			this.rawText = rawText;
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
			$"'{file}' {line}:{col}> {kind} {(Is(TokenKind.Str) ? rawText : ($"'{rawText}'"))}";

		public static readonly string[] puncs = {
			"+", "-", "*", "/", "%",
			"@",
			"{", "}", "(", ")", "[", "]",
			".", ",", ";",
			"=", "->",
			"==", "...",
		};

		public static readonly Dictionary<string, TokenKind> keys = new ()
		{
			{ "decl", TokenKind.DeclKey },
			{ "using", TokenKind.UsingKey },
			{ "struct", TokenKind.StructKey },

			{ "if", TokenKind.IfKey },
			{ "else", TokenKind.ElseKey },

			{ "null", TokenKind.NullKey },
			{ "ret", TokenKind.RetKey },
			{ "ref", TokenKind.RefKey },
			{ "as", TokenKind.AsKey },
		};
	}
}
