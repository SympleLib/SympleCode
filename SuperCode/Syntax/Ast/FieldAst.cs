namespace SuperCode
{
	public class FieldAst: Ast
	{
		public readonly TypeAst type;
		public readonly Token name;
		public readonly Token equal;
		public readonly ExprAst defVal;
		public readonly Token comma;

		public FieldAst(TypeAst ty, Token name, Token comma)
		{
			token = name;

			this.type = ty;
			this.name = name;
			this.comma = comma;
		}

		public FieldAst(TypeAst ty, Token name, Token eql, ExprAst def, Token comma)
		{
			token = name;

			this.type = ty;
			this.name = name;
			this.equal = eql;
			this.defVal = def;
			this.comma = comma;
		}

		public override AstKind kind => AstKind.Field;
	}
}
