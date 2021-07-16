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
	}
}
