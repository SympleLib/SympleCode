namespace CommenSense;

partial class Parser
{
	StmtAst Stmt() => ExprStmt();

	ExprStmtAst ExprStmt() =>
		new ExprStmtAst(Expr());
}