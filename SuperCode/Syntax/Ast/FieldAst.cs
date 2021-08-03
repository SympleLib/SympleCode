namespace SuperCode
{
	public class FieldAst: Ast
	{
		public readonly Token? mutKey;
		public readonly TypeAst type;
		public readonly Token name;
		public readonly Token? equal;
		public readonly ExprAst? defVal;
		public readonly Token comma;

		public FieldAst(Token? mutKey, TypeAst type, Token name, Token comma)
		{
			token = name;

			this.mutKey = mutKey;
			this.type = type;
			this.name = name;
			this.comma = comma;
		}

		public FieldAst(Token? mutKey, TypeAst type, Token name, Token? eql, ExprAst? def, Token comma)
		{
			token = name;

			this.mutKey = mutKey;
			this.type = type;
			this.name = name;
			this.equal = eql;
			this.defVal = def;
			this.comma = comma;
		}

		public override AstKind kind => AstKind.Field;
	}
}
