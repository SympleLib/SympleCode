using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
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
		Eql,
		Arrow,

		Dot,
		Comma,
		Semicol,


		LeftBrace,
		RightBrace,
		LeftBracket,
		RightBracket,
		LeftParen,
		RightParen,

		LeftChevron,
		RightChevron,

		EqlEql,
		NotEql,
		LEql,
		REql,

		DotDotDot,


		DeclKey,
		UsingKey,
		StructKey,

		IfKey,
		ElseKey,

		NullKey,
		RetKey,
		RefKey,
		AsKey,

		MutKey,
		ConstKey,

		PubKey,
		ProtKey,
		PrivKey,

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

		public static readonly string[] puncs =	{
			"+", "-", "*", "/", "%",
			"@", "=", "->",
			".", ",", ";",
			"{", "}", "[", "]", "(", ")",
			"<", ">",
			"==", "!=", "<=", ">=",
			"...",
		};

		public static readonly string[] keys = 
		{
			"decl", "using", "struct",
			"if", "else",
			"null", "ret", "ref", "as",
			"mut", "const",
			"pub", "prot", "priv",
		};
	}
}
