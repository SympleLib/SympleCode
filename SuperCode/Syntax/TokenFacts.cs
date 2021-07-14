namespace SuperCode
{
	public static class TokenFacts
	{
		public static int BinPriority(this TokenKind kind) =>
			kind switch
			{
				TokenKind.Plus or TokenKind.Dash =>
					1,
				TokenKind.Star or TokenKind.Slash or TokenKind.Percent =>
					2,

				_ =>
					0,
			};
	}
}
