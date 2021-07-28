namespace SuperCode
{
	public class IfStmtAst: StmtAst
	{
		public readonly Token ifKey;
		public readonly ExprAst cond;
		public readonly Token arrow;

		public readonly Token open;
		public readonly StmtAst[] then;
		public readonly Token close;

		public readonly Token elseKey;
		public readonly StmtAst elze;

		public IfStmtAst(Token ifKey, ExprAst cond, Token arrow, StmtAst then, Token elseKey, StmtAst elze)
		{
			token = ifKey;

			this.ifKey = ifKey;
			this.cond = cond;
			this.arrow = arrow;
			this.then = new StmtAst[] { then };
			this.elseKey = elseKey;
			this.elze = elze;
		}

		public IfStmtAst(Token ifKey, ExprAst cond, Token open, StmtAst[] then, Token close, Token elseKey, StmtAst elze)
		{
			token = ifKey;

			this.ifKey = ifKey;
			this.cond = cond;
			this.open = open;
			this.then = then;
			this.close = close;
			this.elseKey = elseKey;
			this.elze = elze;
		}

		public override AstKind kind => AstKind.IfStmt;
	}
}
