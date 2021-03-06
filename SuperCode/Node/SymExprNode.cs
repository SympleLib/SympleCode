﻿namespace SuperCode
{
	public class SymExprNode: ExprNode
	{
		public Symbol symbol;

		public SymExprNode(Symbol symbol)
		{
			this.symbol = symbol;
			type = symbol.type;
		}

		public override NodeKind kind => NodeKind.SymExpr;
	}
}
