using System;
using System.Collections.Generic;

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

		public ModuleAst Parse()
		{
			var stmts = new List<StmtAst>();
			while (!current.Is(TokenKind.Eof))
				stmts.Add(Stmt());

			var eof = Next();
			return new ModuleAst(stmts.ToArray(), eof);
		}

		private StmtAst Stmt()
		{
			switch (current.kind)
			{
			case TokenKind.Iden:
				return VarStmt();

			default:
				return ExprStmt();
			}
		}

		private VarStmtAst VarStmt()
		{
			var name = Match(TokenKind.Iden);
			var eql = Match(TokenKind.Eql);
			var init = Expr();

			return new (name, eql, init);
		}

		private ExprStmtAst ExprStmt() =>
			new (Expr());

		private ExprAst Expr() =>
			BinExpr();

		private ExprAst BinExpr(int parentPriority = 0)
		{
			var left = PrimExpr();
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

		private ExprAst PrimExpr()
		{
			switch (current.kind)
			{
			case TokenKind.Num:
				return litExpr();

			default:
				throw new Exception("Expected expr");
			}
		}

		private LitExprAst litExpr() =>
			new (Match(TokenKind.Num));

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
