using LLVMSharp.Interop;
using System;

namespace SuperCode
{
	public abstract class ExprNode
	{
		public LLVMTypeRef type { get; protected init; }

		public abstract Token token { get; }
		public abstract LLVMValueRef CodeGen(LLVMBuilderRef builder);
	}

	public class NumExprNode: ExprNode
	{
		public override Token token => numTok;
		public readonly Token numTok;

		public NumExprNode(Token num)
		{ numTok = num; }

		public override LLVMValueRef CodeGen(LLVMBuilderRef builder)
		{
			int num = int.Parse(numTok.text);
			return LLVMValueRef.CreateConstInt(LLVMTypeRef.Int32, (ulong)num);
		}
	}

	public class BinExprNode: ExprNode
	{
		public override Token token => op;

		public readonly Token op;
		public readonly ExprNode left, right;

		public BinExprNode(ExprNode lhs, Token op, ExprNode rhs)
		{
			this.op = op;
			left = lhs;
			right = rhs;

			type = left.type;
		}

		public override LLVMValueRef CodeGen(LLVMBuilderRef builder)
		{
			switch (op.kind)
			{
			case TokenKind.Plus:
				return builder.BuildAdd(left.CodeGen(builder), right.CodeGen(builder));
			case TokenKind.Minus:
				return builder.BuildSub(left.CodeGen(builder), right.CodeGen(builder));
			case TokenKind.Star:
				return builder.BuildMul(left.CodeGen(builder), right.CodeGen(builder));
			case TokenKind.Slash:
				return builder.BuildSDiv(left.CodeGen(builder), right.CodeGen(builder));
			case TokenKind.Percent:
				return builder.BuildSRem(left.CodeGen(builder), right.CodeGen(builder));

			default:
				Console.Error.WriteLine("Invalid Expression");
				return builder.BuildUnreachable();
			}
		}
	}

	public class Parser
	{
		public readonly Token[] tokens;
		private int pos;

		private Token current => pos < tokens.Length ? tokens[pos] : Token.error;
		
		public Parser(string file)
		{
			var lexer = new Lexer(file);
			tokens = lexer.Lex();
		}

		public ExprNode Parse() =>
			Expr();

		private ExprNode Expr() =>
			BinExpr();

		private ExprNode BinExpr()
		{
			var left = PrimExpr();
			while (current.Is(TokenKind.Plus, TokenKind.Minus, TokenKind.Star, TokenKind.Slash, TokenKind.Percent))
			{
				var op = Next();
				var right = PrimExpr();
				left = new BinExprNode(left, op, right);
			}

			return left;
		}

		private ExprNode PrimExpr()
		{
			switch (current.kind)
			{
			case TokenKind.Number:
				return NumExpr();

			default:
				Console.Error.WriteLine("Expected expression");
				return null;
			}
		}

		private NumExprNode NumExpr() =>
			new (Match(TokenKind.Number));

		private Token Next()
		{
			Token tok = current;
			pos++;
			return tok;
		}

		private Token Match(TokenKind kind)
		{
			if (current.Is(kind))
				return Next();

			Console.Error.WriteLine($"Expected {kind}, got {current}");
			return current;
		}
	}
}
