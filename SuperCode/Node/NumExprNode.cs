using System;
using LLVMSharp.Interop;

namespace SuperCode
{
	public class NumExprNode: ExprNode
	{
		public override Token token => numTok;
		public readonly Token numTok;

		public NumExprNode(Token num)
		{
			type = LLVMTypeRef.Float;
			numTok = num;
		}

		public override LLVMValueRef CodeGen(LLVMBuilderRef builder)
		{
			float num = float.Parse(numTok.text);
			return LLVMValueRef.CreateConstReal(type, num);
		}
	}
}
