namespace CommenSense;

partial class Parser
{
	enum TokenKind
	{
		Unknown = -1,
		Eof,

		Str,
		Int,
		Float,
		Identifier,

		Plus,
		Minus,
		Star,
		Slash,
		Percent,
		SlashDash,
		PercentDash,

		And,

		Eql,
		DotDotDot,

		LeftBrace,
		RightBrace,
		LeftBracket,
		RightBracket,

		Comma,


		TrueKeyword,
		FalseKeyword,

		DeclKeyword,
	}

	record Token(TokenKind kind, string text);
}