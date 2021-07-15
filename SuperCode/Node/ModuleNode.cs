using System;
using LLVMSharp.Interop;

namespace SuperCode
{
	public class ModuleNode: Node
	{
		public readonly StmtNode[] stmts;

		public ModuleNode(StmtNode[] stmts) =>
			this.stmts = stmts;

		public override LLVMValueRef Build(LLVMModuleRef module, LLVMBuilderRef builder)
		{
			foreach (var stmt in stmts)
				stmt.Build(module, builder);
			return builder.BuildRet(LLVMValueRef.CreateConstInt(LLVMTypeRef.Int32, 0, true));
		}
	}
}
