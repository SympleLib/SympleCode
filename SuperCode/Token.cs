namespace SuperCode
{
	public struct Token
	{
		public TokenKind kind { get; init; }
		public string text { get; init; }

		public override string ToString() =>
			$"{kind} `{text}`";
	}

	public enum TokenKind
	{
		Unknown,

		Identifier,
		Number,
	}
}
