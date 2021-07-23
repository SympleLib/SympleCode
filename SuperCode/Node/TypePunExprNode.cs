using LLVMSharp.Interop;

namespace SuperCode
{
	public class TypePunExprNode: ExprNode
	{
		public ExprNode expr;

		public TypePunExprNode(TypeNode type, ExprNode expr)
		{
			this.type = type;
			this.expr = expr;
		}

		public override NodeKind kind => NodeKind.TypePunExpr;
	}
}
