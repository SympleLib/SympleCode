using LLVMSharp.Interop;

namespace SuperCode
{
	public class VarStmtNode: StmtNode
	{
		public readonly string name;
		public readonly ExprNode init;

		public VarStmtNode(string name, ExprNode init)
		{
			this.name = name;
			this.init = init;
		}

		public override NodeKind kind => NodeKind.VarStmt;
		public override LLVMValueRef Build(LLVMBuilderRef builder)
		{
			var ptr = builder.BuildAlloca(LLVMTypeRef.Int32, name);
			builder.BuildStore(init.Build(builder), ptr);
			return ptr;
		}
	}
}
