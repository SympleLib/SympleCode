namespace SuperCode
{
	public class ImplVarMemAst: MemAst
	{
		public readonly Token? mutKey;
		public readonly TypeAst type;
		public readonly Token name;

		public ImplVarMemAst(Token? mutKey, TypeAst type, Token name)
		{
			this.mutKey = mutKey;
			this.type = type;
			this.name = name;
		}

		public override AstKind kind => AstKind.ImplVarMem;
	}
}
