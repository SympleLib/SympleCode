﻿using LLVMSharp.Interop;

namespace SuperCode
{
	public class FNumExprNode: ExprNode
	{
		public readonly double value;

		public FNumExprNode(double val, TypeNode ty)
		{
			value = val;
			type = ty;
		}

		public override NodeKind kind => NodeKind.FNumExpr;
	}
}
