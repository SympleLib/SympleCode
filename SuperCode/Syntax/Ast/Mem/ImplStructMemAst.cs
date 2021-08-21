namespace SuperCode
{
	public class ImplStructMemAst: MemAst
	{
		public readonly Token name;

		public ImplStructMemAst(Token name) =>
			this.name = name;

		public override AstKind kind => AstKind.ImplStructMem;
	}
}
