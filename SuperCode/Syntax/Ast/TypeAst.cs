namespace SuperCode
{
	public class TypeAst: Ast
	{
		public readonly Token type;

		public TypeAst(Token ty) =>
			type = ty;

		public override AstKind kind => AstKind.Type;
	}
}
