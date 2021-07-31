using LLVMSharp.Interop;

namespace SuperCode
{
	public class MemExprNode: ExprNode
	{
		public readonly ExprNode expr;
		public readonly string name;
		public readonly int index;

		public MemExprNode(ExprNode expr, string name, int index, LLVMTypeRef type): base(type)
		{
			this.expr = expr;
			this.name = name;
			this.index = index;
		}

		public override NodeKind kind => NodeKind.MemExpr;
	}
}
