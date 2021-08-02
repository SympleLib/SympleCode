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

		Eql,
		NEql,
		SLtEql,
		SGtEql,
		SGt,
		SLt,

		UDiv,
		UMod,
		ULtEql,
		UGtEql,
		UGt,
		ULt,

		FAdd,
		FSub,
		FMul,
		FDiv,
		FMod,

		FEql,
		FNEql,
		FLtEql,
		FGtEql,
		FGt,
		FLt,

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
