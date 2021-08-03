using System;

using LLVMSharp.Interop;

namespace SuperCode
{
	public partial struct Token
	{
		public int binPriority => kind.BinPriority();
		public bool isPrefix => kind.IsPrefix();
		public bool isTypeAddon => kind.IsTypeAddon();
		public bool isComment => kind.IsComment();

		public bool isBuiltinType {
			get
			{
				if (kind is not TokenKind.Iden)
					return false;

				if (BuiltinTypes.types.ContainsKey(text))
					return true;
				return false;
			}
		}

		public LLVMTypeRef builtinType => BuiltinTypes.types[text];
	}

	public static class TokenFacts
	{
		public const TokenKind firstPunc = TokenKind.Plus;
		public const TokenKind firstKey = TokenKind.ImportKey;

		public static int BinPriority(this TokenKind kind) =>
			kind switch
			{
				TokenKind.Eql => 1,
				TokenKind.EqlEql or TokenKind.NotEql or TokenKind.LeftChevron or TokenKind.RightChevron or TokenKind.LEql or TokenKind.REql => 2,
				TokenKind.Plus or TokenKind.Dash => 3,
				TokenKind.Star or TokenKind.Slash or TokenKind.Percent => 4,
				TokenKind.Dot => 5,

				_ => 0,
			};

		public static bool IsPrefix(this TokenKind kind) =>
			kind is TokenKind.Plus or TokenKind.Dash or TokenKind.Percent or TokenKind.At;

		public static bool IsTypeAddon(this TokenKind kind) =>
			kind is TokenKind.Star;

		public static bool IsComment(this TokenKind kind) =>
			kind is TokenKind.LineComment or TokenKind.LongComment;
	}
}
