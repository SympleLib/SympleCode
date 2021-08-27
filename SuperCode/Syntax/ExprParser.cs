using System.Collections.Generic;

namespace SuperCode
{
	public partial class Parser
	{
		private ExprAst Expr() =>
			BinExpr();

		private ArrExprAst ArrExpr()
		{
			var open = Match(TokenKind.LeftBracket);
			var elements = new List<ElementAst>();
			while (current.kind is not TokenKind.RightBracket and not TokenKind.Eof)
				elements.Add(Element());
			var close = Match(TokenKind.RightBracket);

			return new ArrExprAst(open, elements.ToArray(), close);
		}

		private ExprAst BinExpr(int parentPriority = 0)
		{
			var left = PreExpr();
			while (true)
			{
				int priority = current.binPriority;
				if (priority == 0 || priority < parentPriority)
					break;

				var op = Next();
				var right = BinExpr(priority);
				left = new BinExprAst(op, left, right);
			}

			return left;
		}

		private CastExprAst CastExpr()
		{
			var open = Match(TokenKind.LeftParen);
			var ty = Type();
			var close = Match(TokenKind.RightParen);

			var expr = PreExpr();

			return new CastExprAst(open, ty, close, expr);
		}

		private CallExprAst CallExpr(ExprAst expr)
		{
			var open = Match(TokenKind.LeftParen);
			var args = Args();
			var close = Match(TokenKind.RightParen);
			return new CallExprAst(expr, open, args, close);
		}

		private IndexExprAst IndexExpr(ExprAst expr)
		{
			var open = Match(TokenKind.LeftBracket);
			var index = Expr();
			var close = Match(TokenKind.RightBracket);
			return new IndexExprAst(expr, open, index, close);
		}

		private LitExprAst LitExpr() =>
			new LitExprAst(Next());

		private ParenExprAst ParenExpr()
		{
			var open = Match(TokenKind.LeftParen);
			var expr = Expr();
			var close = Match(TokenKind.RightParen);

			return new ParenExprAst(open, expr, close);
		}

		private ExprAst PreExpr()
		{
			if (current.isPrefix)
				return new PreExprAst(Next(), PostExpr());
			return PostExpr();
		}

		private ExprAst PostExpr()
		{
			var expr = PrimExpr();
			while (true)
				if (current.kind is TokenKind.LeftParen)
					expr = CallExpr(expr);
				else if (current.kind is TokenKind.LeftBracket)
					expr = IndexExpr(expr);
				else
					break;

			return expr;
		}

		private ExprAst PrimExpr()
		{
			switch (current.kind)
			{
			case TokenKind.LeftParen:
				if (HasUntil(TokenKind.RightParen)
					return CastExpr();
				return ParenExpr();
			case TokenKind.Num:
			case TokenKind.Str:
			case TokenKind.Char:
			case TokenKind.Iden:
			case TokenKind.NullKey:
			case TokenKind.TrueKey:
			case TokenKind.FalseKey:
				return LitExpr();
			case TokenKind.LeftBracket:
				if (HasUntil(TokenKind.RightParen, TokenKind.Star))
					return TypePunExpr();
				return ArrExpr();

			default:
				safety.ReportExpectedExpr(current);
				return new NullExprAst();
			}
		}

		private TypePunExprAst TypePunExpr()
		{
			var open = Match(TokenKind.LeftBracket);
			var ty = Type();
			var close = Match(TokenKind.RightBracket);

			var expr = PreExpr();

			return new TypePunExprAst(open, ty, close, expr);
		}
	}
}
