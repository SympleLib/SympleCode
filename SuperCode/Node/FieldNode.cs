﻿using LLVMSharp.Interop;

namespace SuperCode
{
	public class FieldNode: Node, Symbol
	{
		public static readonly FieldNode nullField = new FieldNode(null, false, -1, "NULL FIELD");

		public readonly int index;

		public FieldNode(LLVMTypeRef type, bool mut, int index, string name)
		{
			this.type = type;
			this.mut = mut;
			this.index = index;
			this.name = name;
		}

		public bool mut { get; }
		public string name { get; }
		public Visibility vis => Visibility.Public;
		public LLVMTypeRef type { get; }
		public override NodeKind kind => NodeKind.Field;
	}
}
