﻿using LLVMSharp.Interop;

namespace SuperCode
{
	public class DeclStructMemNode: Node, Symbol, TypePrompt
	{
		public StructMemNode? impl { get; internal set; }

		public DeclStructMemNode(string name) =>
			this.name = name;

		public bool mut => false;
		public string name { get; }
		public Visibility vis => Visibility.Default;
		public LLVMTypeRef type { get; }
		public LLVMTypeRef? realType => impl?.type;
		public override NodeKind kind => NodeKind.DeclStructMem;
	}
}
