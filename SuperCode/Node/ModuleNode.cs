using System;
using LLVMSharp.Interop;

namespace SuperCode
{
	public class ModuleNode: Node
	{
		public readonly string filename;
		public readonly MemNode[] mems;

		public ModuleNode(string file, MemNode[] mems)
		{
			filename = file;
			this.mems = mems;
		}

		public override NodeKind kind => NodeKind.Module;
		public override LLVMValueRef Build(LLVMModuleRef module, LLVMBuilderRef builder)
		{
			foreach (var mem in mems)
				mem.Build(module, builder);
			return null;
		}
	}
}
