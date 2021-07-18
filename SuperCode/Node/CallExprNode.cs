using LLVMSharp.Interop;

namespace SuperCode
{
	public class CallExprNode: ExprNode
	{
		public readonly ExprNode what;
		public readonly ExprNode[] args;

		public CallExprNode(ExprNode what, ExprNode[] args)
		{
			type = what.type.ReturnType;
			this.what = what;
			this.args = args;
		}

		public override NodeKind kind => NodeKind.CallExpr;
	}
}
