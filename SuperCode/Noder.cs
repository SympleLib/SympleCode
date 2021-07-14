using System;

namespace SuperCode
{
	public class Noder
	{
		public readonly Ast tree;

		public Noder(Ast ast) =>
			tree = ast;

		public Node Nodify() =>
			Nodify((ExprAst) tree);

		private ExprNode Nodify(ExprAst expr)
		{
			switch (expr.kind)
			{
			case AstKind.LitExpr:
				return Nodify((LitExprAst) expr);
			case AstKind.BinExpr:
				return Nodify((BinExprAst) expr);

			default:
				throw new InvalidOperationException("Invalid expr");
			}
		}

		private ExprNode Nodify(LitExprAst expr)
		{
			switch (expr.literal.kind)
			{
			case TokenKind.Num:
				return new NumExprNode(ulong.Parse(expr.literal.text));

			default:
				throw new InvalidOperationException("Invalid lit-expr");
			}
		}

		private BinExprNode Nodify(BinExprAst expr)
		{
			var left = Nodify(expr.left);

			BinOp op;
			switch (expr.op.kind)
			{
			case TokenKind.Plus:
				op = BinOp.Add;
				goto BinExpr;
			case TokenKind.Dash:
				op = BinOp.Sub;
				goto BinExpr;
			case TokenKind.Star:
				op = BinOp.Mul;
				goto BinExpr;
			case TokenKind.Slash:
				op = BinOp.Div;
				goto BinExpr;
			case TokenKind.Percent:
				op = BinOp.Mod;
				goto BinExpr;

			default:
				throw new InvalidOperationException("Invalid lit-expr");
			}

		BinExpr:
			var right = Nodify(expr.right);
			return new BinExprNode(op, left, right);
		}
	}
}
