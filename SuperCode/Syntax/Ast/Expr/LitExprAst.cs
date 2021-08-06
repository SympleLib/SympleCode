using System;

namespace SuperCode
{
	public class LitExprAst: ExprAst
	{
		public readonly Token literal;

		public LitExprAst(Token lit) =>
			token = literal = lit;

		public override AstKind kind => AstKind.LitExpr;
	}
}
