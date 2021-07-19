using System;

using LLVMSharp.Interop;

namespace SuperCode
{
	public enum BinOp
	{
		Add,
		Sub,
		Mul,
		SDiv,
		SMod,
		UDiv,
		UMod,

		FAdd,
		FSub,
		FMul,
		FDiv,
		FMod,
	}

	public class BinExprNode: ExprNode
	{
		public readonly BinOp op;
		public readonly ExprNode left, right;

		public BinExprNode(BinOp op, ExprNode lhs, ExprNode rhs)
		{
			this.op = op;
			type = lhs.type;
			left = lhs;
			right = rhs;
		}

		public override NodeKind kind => NodeKind.BinExpr;
	}
}
