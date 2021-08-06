namespace SuperCode
{
	public class WhileStmtAst: StmtAst
	{
		public readonly Token key;
		public readonly ExprAst cond;

		public readonly Token? arrow;
		public readonly Token? open;
		public readonly StmtAst[] then;
		public readonly Token? close;

		public WhileStmtAst(Token key, ExprAst cond, Token arrow, StmtAst then)
		{
			this.key = key;
			this.cond = cond;
			this.arrow = arrow;
			this.then = new StmtAst[] { then };
		}

		public WhileStmtAst(Token key, ExprAst cond, Token open, StmtAst[] then, Token close)
		{
			this.key = key;
			this.cond = cond;
			this.open = open;
			this.then = then;
			this.close = close;
		}

		public override AstKind kind => AstKind.WhileStmt;
	}
}
