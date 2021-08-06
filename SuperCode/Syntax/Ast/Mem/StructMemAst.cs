namespace SuperCode
{
	public class StructMemAst: MemAst
	{
		public readonly Token key;
		public readonly Token name;
		public readonly Token open;
		public readonly FieldAst[] fields;
		public readonly Token close;

		public StructMemAst(Token key, Token name, Token open, FieldAst[] fields, Token close)
		{
			token = key;

			this.key = key;
			this.name = name;
			this.open = open;
			this.fields = fields;
			this.close = close;
		}

		public override AstKind kind => AstKind.StructMem;
	}
}
