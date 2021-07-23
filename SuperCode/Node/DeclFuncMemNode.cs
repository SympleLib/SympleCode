﻿using LLVMSharp.Interop;

namespace SuperCode
{
	public class DeclFuncMemNode: MemNode, Symbol
	{
		public readonly ParamNode[] paramz;

		public DeclFuncMemNode(TypeNode type, string name, ParamNode[] paramz)
		{
			this.type = type;
			this.name = name;
			this.paramz = paramz;
		}

		public string name { get; init; }
		public bool unsigned { get; init; }
		public TypeNode type { get; init; }
		public override NodeKind kind => NodeKind.DeclFuncMem;
	}
}
