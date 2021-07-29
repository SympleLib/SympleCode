using System;

namespace SuperCode
{
	// Imagine if I typed the whole thing out: DeclareFunctionMemberAbstractSyntaxTree. Ope, no need to imagine!
	public class DeclFuncMemAst: MemAst
	{
		public readonly Token key;
		public readonly TypeAst retType;
		public readonly Token name;
		public readonly Token asmTag;
		public readonly Token open;
		public readonly FieldAst[] paramz;
		public readonly Token close;

		public DeclFuncMemAst(Token key, TypeAst retType, Token name, Token asmTag, Token open, FieldAst[] paramz, Token close, Token semi): base(semi)
		{
			token = key;

			this.key = key;
			this.retType = retType;
			this.name = name;
			this.asmTag = asmTag;
			this.open = open;
			this.paramz = paramz;
			this.close = close;
		}

		public override AstKind kind => AstKind.DeclFuncMem;
	}
}
