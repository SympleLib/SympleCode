﻿namespace SuperCode
{
	public class VarStmtAst: StmtAst
	{
		public readonly Token? mutKey;
		public readonly TypeAst type;
		public readonly Token name;
		public readonly Token? eql;
		public readonly ExprAst? init;

		public VarStmtAst(Token? mutKey, TypeAst type, Token name, Token? eql, ExprAst? init, Token semi): base(semi)
		{
			token = eql?? name;

			this.mutKey = mutKey;
			this.type = type;
			this.name = name;
			this.name = name;
			this.eql = eql;
			this.init = init;
		}

		public override AstKind kind => AstKind.VarStmt;
	}
}