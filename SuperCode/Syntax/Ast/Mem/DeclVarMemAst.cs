namespace SuperCode
{
	public class DeclVarMemAst: MemAst
	{
		public readonly Token key;
		public readonly Token? mutKey;
		public readonly TypeAst type;
		public readonly Token name;

		public DeclVarMemAst(Token key, Token? mutKey, TypeAst type, Token name, Token semi): base(semi)
		{
			this.key = key;
			this.mutKey = mutKey;
			this.type = type;
			this.name = name;
		}

		public override AstKind kind => AstKind.DeclVarMem;
	}
}
