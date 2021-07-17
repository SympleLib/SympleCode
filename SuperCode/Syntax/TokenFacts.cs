using System;

using LLVMSharp.Interop;

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

		public LLVMTypeRef builtinType {
			get
			{

				if (kind != TokenKind.Iden)
					throw new InvalidOperationException("Token is not built-in type");

				switch (text)
				{
				case "void":
					return LLVMTypeRef.Void;
				case "byte":
					return LLVMTypeRef.Int8;
				case "short":
					return LLVMTypeRef.Int16;
				case "int":
					return LLVMTypeRef.Int32;
				case "long":
					return LLVMTypeRef.Int64;

				case "char":
					return LLVMTypeRef.Int8;
				case "wchar":
					return LLVMTypeRef.Int32;
				case "bool":
					return LLVMTypeRef.Int1;

				default:
					throw new InvalidOperationException("Token is not built-in type");
				}
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
