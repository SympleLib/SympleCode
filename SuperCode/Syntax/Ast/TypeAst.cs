﻿using LLVMSharp.Interop;

namespace SuperCode
{
	public class TypeAst: Ast
	{
		public readonly Token baze;
		public readonly Token[] addons;

		public TypeAst(Token baze, Token[] addons)
		{
			token = baze;

			this.baze = baze;
			this.addons = addons;
		}

		public bool unsigned => Type
		public override AstKind kind => AstKind.Type;
	}
}
