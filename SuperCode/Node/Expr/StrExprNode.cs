using LLVMSharp.Interop;

namespace SuperCode
{
	public class StrExprNode: ExprNode
	{
		public readonly string str;

		public StrExprNode( LLVMTypeRef type, string str): base(type) =>
			this.str = str;

		public override NodeKind kind => NodeKind.StrExpr;
	}
}
