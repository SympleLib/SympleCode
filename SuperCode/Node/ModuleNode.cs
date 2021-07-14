using System;
using LLVMSharp.Interop;

namespace SuperCode
{
	public class ModuleNode: Node
	{
		public readonly StmtNode[] stmts;

		public ModuleNode(StmtNode[] stmts) =>
			this.stmts = stmts;

		public override LLVMValueRef Build(LLVMBuilderRef builder)
		{
			foreach (var stmt in stmts)
				stmt.Build(builder);
			return builder.BuildRetVoid();
		}
	}
}
