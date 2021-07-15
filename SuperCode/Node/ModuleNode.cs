using System;
using LLVMSharp.Interop;

namespace SuperCode
{
	public class ModuleNode: Node
	{
		public readonly MemNode[] mems;

		public ModuleNode(MemNode[] mems) =>
			this.mems = mems;

		public override NodeKind kind => NodeKind.Module;
		public override LLVMValueRef Build(LLVMModuleRef module, LLVMBuilderRef builder)
		{
			foreach (var mem in mems)
				mem.Build(module, builder);
			return null;
		}
	}
}
