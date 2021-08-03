using System;
using System.Collections.Generic;

using LLVMSharp.Interop;

namespace SuperCode
{
	public class ModuleNode: Node
	{
		public LLVMModuleRef llmodule { get; internal set; }
		public readonly string filename;
		public readonly Node[] mems;
		public readonly Dictionary<string, Symbol> symbols;

		public ModuleNode(string file, Node[] mems, Dictionary<string, Symbol> symbols)
		{
			filename = file;
			this.mems = mems;
			this.symbols = symbols;
		}

		public override NodeKind kind => NodeKind.Module;
	}
}
