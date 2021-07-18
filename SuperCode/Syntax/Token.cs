using System.Collections.Generic;

namespace SuperCode
{
	public enum TokenKind
	{
		Unknown,
		Eof,

		Iden,
		Num,

		Plus,
		Dash,
		Star,
		Slash,
		Percent,

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

		public bool Is(params TokenKind[] kinds)
		{
			foreach (var kind in kinds)
				if (this.kind == kind)
					return true;
			return false;
		}

		public override string ToString() =>
			$"'{file}' {line}:{col}> {kind} `{text}`";

		public static readonly string[] puncs = {
			"+", "-", "*", "/", "%",
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
