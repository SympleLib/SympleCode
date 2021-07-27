using LLVMSharp.Interop;

namespace SuperCode
{
	public enum StrType
	{
		Short,
		Unicode,
		Wide,
	}

	public class StrExprNode: ExprNode
	{
		public readonly StrType strType;
		public readonly string str;

		public StrExprNode(StrType strType, string str)
		{
			this.strType = strType;
			this.str = str;
			switch (strType)
			{
			case StrType.Short:
				type = LLVMTypeRef.Int8.Ref();
				break;
			case StrType.Unicode:
				type = LLVMTypeRef.Int16.Ref();
				break;
			case StrType.Wide:
				type = LLVMTypeRef.Int32.Ref();
				break;
			}
		}

		public override NodeKind kind => NodeKind.StrExpr;
	}
}
