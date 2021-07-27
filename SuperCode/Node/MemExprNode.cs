using LLVMSharp.Interop;

namespace SuperCode
{
	public class MemExprNode: ExprNode
	{
		public ExprNode expr;
		public string name;
		public int index;

		public MemExprNode(ExprNode expr, string name, int index, LLVMTypeRef type)
		{
			this.expr = expr;
			this.name = name;
			this.index = index;
			this.type = type;
		}

		public override NodeKind kind => NodeKind.MemExpr;
	}
}
