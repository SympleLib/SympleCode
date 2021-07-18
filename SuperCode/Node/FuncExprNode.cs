namespace SuperCode
{
	public class FuncExprNode: ExprNode
	{
		public FuncMemNode symbol;

		public FuncExprNode(FuncMemNode symbol)
		{
			this.symbol = symbol;
			type = symbol.type;
		}

		public override NodeKind kind => NodeKind.FuncExpr;
	}
}
