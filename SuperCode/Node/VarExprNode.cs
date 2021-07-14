using System;

using LLVMSharp.Interop;

namespace SuperCode
{
	public class VarExprNode: ExprNode
	{
		public readonly string name;

		public VarExprNode(string name) =>
			this.name = name;

		public override LLVMValueRef Build(LLVMBuilderRef builder) =>
			throw new NotImplementedException();
	}
}
