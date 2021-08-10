namespace SuperCode
{
	public class ImplFuncMemAst: MemAst
	{
		public readonly TypeAst retType;
		public readonly Token name;
		public readonly Token? asmTag;
		public readonly FieldAst[] paramz;
		public readonly Token? vaArg;

		public ImplFuncMemAst(TypeAst retType, Token name, Token? asmTag, FieldAst[] paramz, Token? vaArg)
		{
			token = name;

			this.retType = retType;
			this.name = name;
			this.asmTag = asmTag;
			this.paramz = paramz;
			this.vaArg = vaArg;
		}

		public override AstKind kind => AstKind.ImplFuncMem;
	}
}
