namespace SuperCode
{
	public class VarExprNode: ExprNode
	{
		public readonly VarStmtNode symbol;

		public VarExprNode(VarStmtNode symbol)
		{
			this.symbol = symbol;
			type = symbol.type;
		}

		public override NodeKind kind => NodeKind.VarExpr;
	}
}
