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
	}

	record Token(TokenKind kind, string text);
}