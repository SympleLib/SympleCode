namespace SuperCode
{
	public partial struct Token
	{
		public int binPriority => kind.BinPriority();
		public bool isBuiltinType {
			get
			{
				if (kind != TokenKind.Iden)
					return false;

				if (text is "void" or "byte" or "short" or "int" or "long" or
					"char" or "wchar" or "bool")
					return true;
				return false;
			}
		}
	}

	public static class TokenFacts
	{
		public const TokenKind firstPunc = TokenKind.Plus;
		public const TokenKind firstKey = TokenKind.VarKey;

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
