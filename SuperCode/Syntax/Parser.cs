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
			var mems = new List<MemAst>();
			while (!current.Is(TokenKind.Eof))
				mems.Add(Mem());

			var eof = Next();
			return new ModuleAst(mems.ToArray(), eof);
		}

		private MemAst Mem()
		{
			switch (current.kind)
			{
			case TokenKind.FuncKey:
				return FuncMem();

			default:
				throw new InvalidOperationException("Invlid mem");
			}
		}

		private FuncMemAst FuncMem()
		{
			var key = Match(TokenKind.FuncKey);
			var name = Match(TokenKind.Iden);
			var arrow = Match(TokenKind.Arrow);

			var stmts = new List<StmtAst>()
			{ Stmt() };

			return new FuncMemAst(key, name, arrow, stmts.ToArray());
		}

		private StmtAst Stmt()
		{
			switch (current.kind)
			{
			case TokenKind.VarKey:
				return VarStmt();

			default:
				return ExprStmt();
			}
		}

		private VarStmtAst VarStmt()
		{
			var key = Match(TokenKind.VarKey);
			var name = Match(TokenKind.Iden);
			var eql = Match(TokenKind.Eql);
			var init = Expr();
			var semi = Match(TokenKind.Semicol);

			return new (key, name, eql, init, semi);
		}

		private ExprStmtAst ExprStmt() =>
			new (Expr(), Match(TokenKind.Semicol));

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
			case TokenKind.Iden:
				return litExpr();

			default:
				throw new InvalidOperationException("Expected expr");
			}
		}

		private LitExprAst litExpr() =>
			new (Next());

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

			throw new InvalidOperationException($"Expected {kind}, got {current}");
		}
	}
}
