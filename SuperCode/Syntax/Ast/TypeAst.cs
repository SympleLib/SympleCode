namespace SuperCode
{
	public class TypeAst: Ast
	{
		public readonly Token baze;
		public readonly Token? open;
		public readonly TypeAst[] args;
		public readonly Token? close;
		public readonly Token[] addons;
		public readonly Token? refTok;

		public TypeAst(Token baze, Token? open, TypeAst[] args, Token? close, Token[] addons, Token? refTok)
		{
			token = baze;

			this.baze = baze;
			this.open = open;
			this.args = args;
			this.close = close;
			this.addons = addons;
			this.refTok = refTok;
		}

		public override AstKind kind => AstKind.Type;
	}
}
