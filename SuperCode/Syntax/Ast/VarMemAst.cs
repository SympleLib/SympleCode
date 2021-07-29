namespace SuperCode
{
	public class VarMemAst: MemAst
	{
		public readonly TypeAst type;
		public readonly Token name;
		public readonly Token eql;
		public readonly ExprAst init;

		public VarMemAst(TypeAst type, Token name, Token eql, ExprAst init, Token semi): base(semi)
		{
			token = eql == default ? name : eql;

			this.type = type;
			this.name = name;
			this.eql = eql;
			this.init = init;
		}

		public override AstKind kind => AstKind.VarMem;
	}
}
