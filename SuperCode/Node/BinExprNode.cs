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

		Index,
	}

	public class BinExprNode: ExprNode
	{
		public readonly BinOp op;
		public readonly ExprNode left, right;

		public BinExprNode(BinOp op, ExprNode lhs, ExprNode rhs, LLVMTypeRef type): base(type)
		{
			this.op = op;
			left = lhs;
			right = rhs;
		}

		public override bool mut => op is BinOp.Index && left.mut;
		public override NodeKind kind => NodeKind.BinExpr;
	}
}
