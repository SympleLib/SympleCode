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

		Eql,
		Gt,
		Lt,
	}

	public class BinExprNode: ExprNode
	{
		public readonly BinOp op;
		public readonly ExprNode left, right;

		public BinExprNode(BinOp op, ExprNode lhs, ExprNode rhs, LLVMTypeRef type)
		{
			this.op = op;
			this.type = type;
			left = lhs;
			right = rhs;
		}

		public override NodeKind kind => NodeKind.BinExpr;
	}
}
