using System;
using LLVMSharp.Interop;

namespace SuperCode
{
	public abstract class ExprNode: Node
	{ public LLVMTypeRef type { get; protected init; } }

	public class NumExprNode: ExprNode
	{
		public override Token token => numTok;
		public readonly Token numTok;

		public NumExprNode(Token num)
		{ numTok = num; }

		public override LLVMValueRef CodeGen(LLVMBuilderRef builder)
		{
			float num = float.Parse(numTok.text);
			return LLVMValueRef.CreateConstReal(LLVMTypeRef.Float, num);
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
				return builder.BuildFAdd(left.CodeGen(builder), right.CodeGen(builder));
			case TokenKind.Minus:
				return builder.BuildFSub(left.CodeGen(builder), right.CodeGen(builder));
			case TokenKind.Star:
				return builder.BuildFMul(left.CodeGen(builder), right.CodeGen(builder));
			case TokenKind.Slash:
				return builder.BuildFDiv(left.CodeGen(builder), right.CodeGen(builder));
			case TokenKind.Percent:
				return builder.BuildFRem(left.CodeGen(builder), right.CodeGen(builder));

			default:
				Console.Error.WriteLine("Invalid Expression");
				return builder.BuildUnreachable();
			}
		}
	}
}
