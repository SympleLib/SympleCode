namespace SuperCode
{
	public class SymExprNode: ExprNode
	{
		public readonly Symbol symbol;

		public SymExprNode(Symbol symbol): base(symbol.type) =>
			this.symbol = symbol;

		public override bool mut => symbol.mut;
		public override NodeKind kind => NodeKind.SymExpr;
	}
}
