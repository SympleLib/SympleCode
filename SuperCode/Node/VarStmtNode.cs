using System.Collections.Generic;

using LLVMSharp.Interop;

namespace SuperCode
{
	public class VarStmtNode: StmtNode
	{
		// TODO: donut chez
		public static readonly Dictionary<string, LLVMValueRef> vars = new ();

		public readonly string name;
		public readonly ExprNode init;

		public VarStmtNode(string name, ExprNode init)
		{
			this.name = name;
			this.init = init;
		}

		public override NodeKind kind => NodeKind.VarStmt;
		public override LLVMValueRef Build(LLVMModuleRef module, LLVMBuilderRef builder)
		{
			var var = builder.BuildAlloca(LLVMTypeRef.Int32);
			var val = init.Build(module, builder);
			builder.BuildStore(val, var);

			vars.Add(name, var);

			return var;
		}
	}
}
