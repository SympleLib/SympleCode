namespace SuperCode
{
	public class ElementAst: Ast
	{
		public readonly ExprAst value;
		public readonly Token comma;

		public ElementAst(ExprAst value, Token comma)
		{
			this.value = value;
			this.comma = comma;
		}

		public override AstKind kind => AstKind.Element;
	}
}
