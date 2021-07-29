namespace SuperCode
{
	public class VarMemAst: MemAst
	{
		public TypeAst type;
		public Token name;
		public Token eql;
		public ExprAst init;

		public VarMemAst(TypeAst type, Token name, Token eql, ExprAst init, Token semi): base(semi)
		{
			this.type = type;
			this.name = name;
			this.eql = eql;
			this.init = init;
		}

		public override AstKind kind => AstKind.VarMem;
	}
}
