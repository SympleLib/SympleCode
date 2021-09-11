namespace CommenSense;

static class Resolver
{
	public static Ast Resolve(Ast ast)
	{
		if (ast is ExprAst expr)
			return Resolve(expr);
		throw new Exception("Resolve, involve, disengaged");
	}

	public static ExprAst Resolve(ExprAst ast)
	{
		if (ast is BiExprAst biExpr)
			return Resolve(biExpr);
		if (ast is IntLiteralExprAst)
			return ast;
		throw new Exception("Resolve, involve, disengaged");
	}

	public static BiExprAst Resolve(BiExprAst ast)
	{
		ExprAst left = Resolve(ast.left);
		ExprAst right = Resolve(ast.right);

		return new BiExprAst(ast.op, left, right);
	}
}