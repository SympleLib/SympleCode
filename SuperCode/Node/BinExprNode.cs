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
	}
}
