using System;
using LLVMSharp.Interop;

namespace SuperCode
{
	public class ModuleNode: Node
	{
		public readonly string filename;
		public readonly Node[] mems;

		public ModuleNode(string file, Node[] mems)
		{
			filename = file;
			this.mems = mems;
		}

		public override NodeKind kind => NodeKind.Module;
	}
}
