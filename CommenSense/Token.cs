namespace CommenSense;

partial class Parser
{
	enum TokenKind
	{
		Unknown = -1,
		Eof,

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

		LeftBrace,
		RightBrace,
	}

	record Token(TokenKind kind, string text);
}