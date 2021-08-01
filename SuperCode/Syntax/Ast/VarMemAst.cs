﻿namespace SuperCode
{
	public class VarMemAst: MemAst
	{
		public readonly Token? vis;
		public readonly Token? mutKey;
		public readonly TypeAst type;
		public readonly Token name;
		public readonly Token? eql;
		public readonly ExprAst init;

		public VarMemAst(Token? vis, Token? mutKey, TypeAst type, Token name, Token? eql, ExprAst init, Token semi): base(semi)
		{
			token = eql?? name;

			this.vis = vis;
			this.mutKey = mutKey;
			this.type = type;
			this.name = name;
			this.eql = eql;
			this.init = init;
		}

		public override AstKind kind => AstKind.VarMem;
	}
}
