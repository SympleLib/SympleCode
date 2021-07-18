using System;

namespace SuperCode
{
	// Imagine if I typed the whole thing out: DeclareFunctionMemberAbstractSyntaxTree. Ope, no need to imagine!
	public class DeclFuncMemAst: MemAst
	{
		public readonly Token key;
		public readonly TypeAst retType;
		public readonly Token name;
		public readonly Token open;
		public readonly ParamAst[] paramz;
		public readonly Token close;
		public readonly Token semicol;

		public DeclFuncMemAst(Token key, TypeAst retType, Token name, Token open, ParamAst[] paramz, Token close, Token semi)
		{
			this.key = key;
			this.retType = retType;
			this.name = name;
			this.open = open;
			this.paramz = paramz;
			this.close = close;
			this.semicol = semi;
		}

		public override AstKind kind => AstKind.DeclFuncMem;
	}
}
