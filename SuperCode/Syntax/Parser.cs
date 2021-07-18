using System;
using System.Collections.Generic;

namespace SuperCode
{
	public class Parser
	{
		public readonly Lexer lexer;
		public readonly Token[] tokens;
		private int pos;

		private Token current => pos < tokens.Length ? tokens[pos] : default;
		
		public Parser(string file)
		{
			lexer = new Lexer(file);
			tokens = lexer.Lex();
		}

		public ModuleAst Parse()
		{
			var mems = new List<MemAst>();
			while (!current.Is(TokenKind.Eof))
				mems.Add(Mem());

			var eof = Next();
			return new (lexer.path, mems.ToArray(), eof);
		}

		private TypeAst Type() =>
			new (Next());

		private MemAst Mem()
		{
			switch (current.kind)
			{
			default:
				if (current.isBuiltinType)
					return FuncMem();
				throw new InvalidOperationException("Invlid mem");
			}
		}

		private FuncMemAst FuncMem()
		{
			var ty = Type();
			var name = Match(TokenKind.Iden);
			var openArg = Match(TokenKind.LeftParen);
			var closeArg = Match(TokenKind.RightParen);
			var stmts = new List<StmtAst>();

			if (current.Is(TokenKind.Arrow))
			{
				var arrow = Next();

				stmts.Add(Stmt());
				return new FuncMemAst(ty, name, openArg, closeArg, arrow, stmts.ToArray());
			}

			var open = Match(TokenKind.LeftBrace);
			while (!current.Is(TokenKind.RightBrace))
			{
				if (current.Is(TokenKind.Eof))
					throw new InvalidOperationException("Finish ur code dude");

				stmts.Add(Stmt());
			}

			var close = Next();
			return new FuncMemAst(ty, name, openArg, closeArg, open, close, stmts.ToArray());
		}

		private StmtAst Stmt()
		{
			switch (current.kind)
			{
			case TokenKind.RetKey:
				return RetStmt();

			default:
				if (current.isBuiltinType)
					return VarStmt();
				return ExprStmt();
			}
		}

		private RetStmtAst RetStmt()
		{
			var key = Match(TokenKind.RetKey);
			var val = Expr();
			var semi = Match(TokenKind.Semicol);

			return new (key, val, semi);
		}

		private VarStmtAst VarStmt(TypeAst ty = null)
		{
			if (ty is null || current.isBuiltinType)
				ty = Type();
			var name = Match(TokenKind.Iden);
			var eql = Match(TokenKind.Eql);
			var init = Expr();
			var semi = Match(TokenKind.Semicol);

			return new (ty, name, eql, init, semi);
		}

		private ExprStmtAst ExprStmt() =>
			new (Expr(), Match(TokenKind.Semicol));

		private ExprAst Expr() =>
			BinExpr();

		private ExprAst BinExpr(int parentPriority = 0)
		{
			var left = MaybeCallExpr();
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

		private ExprAst[] Args()
		{
			var args = new List<ExprAst>();
			while (!current.Is(TokenKind.RightParen))
			{
				if (current.Is(TokenKind.Eof))
					throw new InvalidOperationException("Invalid eof");
				args.Add(Expr());
				if (current.Is(TokenKind.Comma))
					Next();
			}

			return args.ToArray();
		}

		private ExprAst MaybeCallExpr()
		{
			var expr = PrimExpr();
			while (current.Is(TokenKind.LeftParen))
			{
				var open = Next();
				var args = Args();
				var close = Match(TokenKind.RightParen);
				expr = new CallExprAst(expr, open, args, close);
			}

			return expr;
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
