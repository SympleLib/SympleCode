using System;

namespace SuperCode
{
	public class Parser
	{
		public readonly Token[] tokens;
		private int pos;

		private Token current => pos < tokens.Length ? tokens[pos] : Token.error;
		
		public Parser(string file)
		{
			var lexer = new Lexer(file);
			tokens = lexer.Lex();
		}

		public Ast Parse() =>
			Expr();

		private ExprAst Expr() =>
			BinExpr();

		private ExprAst BinExpr(int parentPri = 0)
		{
			var left = PrimExpr();
			while (true)
			{
				int priority = current.kind.BinPri();
				if (priority == 0 || priority < parentPri)
					break;

				var op = Next();
				var right = BinExpr(priority);
				left = new BinExprAst(op, left, right);
			}

			return left;
		}

		private ExprAst PrimExpr()
		{
			switch (current.kind)
			{
			case TokenKind.Number:
				return litExpr();

			default:
				Console.Error.WriteLine("Expected expression");
				return null;
			}
		}

		private LitExprAst litExpr() =>
			new (Match(TokenKind.Number));

		private Token Next()
		{
			Token tok = current;
			pos++;
			return tok;
		}

		private Token Match(TokenKind kind)
		{
			if (current.Is(kind))
				return Next();

			Console.Error.WriteLine($"Expected {kind}, got {current}");
			return current;
		}
	}
}
