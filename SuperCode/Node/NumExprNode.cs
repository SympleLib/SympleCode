﻿using LLVMSharp.Interop;

namespace SuperCode
{
	public class NumExprNode: ExprNode
	{
		public readonly ulong value;

		public NumExprNode(ulong val, TypeNode ty)
		{
			value = val;
			type = ty;
		}

		public override NodeKind kind => NodeKind.NumExpr;
	}
}
