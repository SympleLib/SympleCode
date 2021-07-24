using System;

using LLVMSharp.Interop;

namespace SuperCode
{
	public partial struct Token
	{
		public int binPriority => kind.BinPriority();
		public bool isPrefix => kind.IsPrefix();
		public bool isTypeAddon => kind.IsTypeAddon();

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

		public LLVMTypeRef builtinType {
			get
			{
				if (kind != TokenKind.Iden)
					throw new InvalidOperationException("Token is not built-in type");
				return BuiltinTypes.types[text];
			}
		}
	}

	public static class TokenFacts
	{
		public const TokenKind firstPunc = TokenKind.Plus;
		public const TokenKind firstKey = TokenKind.DeclKey;

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

		public static bool IsPrefix(this TokenKind kind) =>
			kind is TokenKind.Plus or TokenKind.Dash or TokenKind.Percent or TokenKind.At;

		public static bool IsTypeAddon(this TokenKind kind) =>
			kind is TokenKind.Star;
	}
}
