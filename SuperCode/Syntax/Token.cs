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

		Eql,

		Semicol,

		VarKey,

		Count,
	}

	public struct Token
	{
		public static readonly Token error = new(TokenKind.Unknown, "", "", 0);
		public int binPriority => kind.BinPriority();

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

		public static readonly string[] puncs = {
			"+", "-", "*", "/", "%",
			"=",
			";",
		};

		public static readonly Dictionary<string, TokenKind> keys = new ()
		{
			{ "var", TokenKind.VarKey },
		};
	}
}
