using System;

using LLVMSharp.Interop;

namespace SuperCode
{
	public enum BinOp
	{
		Add,
		Sub,
		Mul,
		Div,
		Mod,
	}

	public class BinExprNode: ExprNode
	{
		public readonly BinOp op;
		public readonly ExprNode left, right;

		public BinExprNode(BinOp op, ExprNode lhs, ExprNode rhs)
		{
			this.op = op;
			left = lhs;
			right = rhs;
		}

		public override NodeKind kind => NodeKind.BinExpr;
		public override LLVMValueRef Build(LLVMModuleRef module, LLVMBuilderRef builder)
		{
			var lhs = left.Build(module, builder);
			var rhs = right.Build(module, builder);

			switch (op)
			{
			case BinOp.Add:
				return builder.BuildAdd(lhs, rhs);
			case BinOp.Sub:
				return builder.BuildSub(lhs, rhs);
			case BinOp.Mul:
				return builder.BuildMul(lhs, rhs);
			case BinOp.Div:
				return builder.BuildSDiv(lhs, rhs);
			case BinOp.Mod:
				return builder.BuildSRem(lhs, rhs);

			default:
				throw new InvalidOperationException("Invalid bin-op");
			}
		}
	}
}
