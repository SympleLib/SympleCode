﻿using LLVMSharp.Interop;

namespace SuperCode
{
	public class StrExprNode: ExprNode
	{
		public readonly string str;

		public StrExprNode(string str)
		{
			this.str = str;
			type = TypeNode.i8.Ref();
		}

		public override NodeKind kind => NodeKind.StrExpr;
	}
}
