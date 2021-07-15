using System;

using LLVMSharp.Interop;

namespace SuperCode
{
	public class FuncMemNode: MemNode
	{
		public readonly string name;
		public readonly StmtNode[] stmts;

		public FuncMemNode(string name, StmtNode[] stmts)
		{
			this.name = name;
			this.stmts = stmts;
		}

		public override NodeKind kind => NodeKind.FuncMem;
		public override LLVMValueRef Build(LLVMModuleRef module, LLVMBuilderRef builder)
		{
			var ty = LLVMTypeRef.CreateFunction(LLVMTypeRef.Void, Array.Empty<LLVMTypeRef>());
			var fn = module.AddFunction(name, ty);
			var entry = fn.AppendBasicBlock("Entry");
			builder.PositionAtEnd(entry);
			foreach (var stmt in stmts)
				stmt.Build(module, builder);
			builder.BuildRetVoid();

			return fn;
		}
	}
}
