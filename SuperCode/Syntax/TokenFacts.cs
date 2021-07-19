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

				if (text is "void" or "byte" or "short" or "int" or "long" or
					"float" or "double" or
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

				case "float":
					return LLVMTypeRef.Float;
				case "double":
					return LLVMTypeRef.Double;

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
