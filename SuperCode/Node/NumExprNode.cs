using LLVMSharp.Interop;

namespace SuperCode
{
	public class NumExprNode: ExprNode
	{
		public readonly int value;

		public NumExprNode(LitExprAst expr, int val)
		{
			value = val;
			type = LLVMTypeRef.Int32;
		}

		public override NodeKind kind => NodeKind.NumExpr;
	}
}
