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

		public ExprNode Parse() =>
			Expr();

		private ExprNode Expr() =>
			BinExpr();

		private ExprNode BinExpr()
		{
			var left = PrimExpr();
			while (current.Is(TokenKind.Plus, TokenKind.Minus, TokenKind.Star, TokenKind.Slash, TokenKind.Percent))
			{
				var op = Next();
				var right = PrimExpr();
				left = new BinExprNode(left, op, right);
			}

			return left;
		}

		private ExprNode PrimExpr()
		{
			switch (current.kind)
			{
			case TokenKind.Number:
				return NumExpr();

			default:
				Console.Error.WriteLine("Expected expression");
				return null;
			}
		}

		private NumExprNode NumExpr() =>
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
