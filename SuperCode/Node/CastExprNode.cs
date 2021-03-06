﻿using LLVMSharp.Interop;

namespace SuperCode
{
	public class CastExprNode: ExprNode
	{
		public readonly ExprNode value;

		public CastExprNode(ExprNode val, LLVMTypeRef to)
		{
			this.value = val;
			type = to;
		}
		
		public override NodeKind kind => NodeKind.CastExpr;
	}
}
