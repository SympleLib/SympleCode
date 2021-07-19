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
		private Token next => pos + 1 < tokens.Length ? tokens[pos + 1] : default;
		
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

		private TypeAst Type()
		{
			var baze = Next();
			var addons = new List<Token>();
			while (current.isTypeAddon)
				addons.Add(Next());

			return new (baze, addons.ToArray());
		}

		private ParamAst Param(TypeAst ty = null)
		{
			if (ty is null || current.isBuiltinType)
				ty = Type();
			
			var name = current.Is(TokenKind.Iden) ? Next() : default;
			Token comma = default;
			if (current.Is(TokenKind.Eql))
			{
				var eql = Next();
				var defVal = Expr();

				if (current.Is(TokenKind.Comma))
					comma = Next();

				return new (ty, name, eql, defVal, comma);
			}

			if (current.Is(TokenKind.Comma))
				comma = Next();

			return new (ty, name, comma);
		}

		private MemAst Mem()
		{
			switch (current.kind)
			{
			case TokenKind.DeclKey:
				return DeclFuncMem();

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
			var paramz = new List<ParamAst>();
			while (!current.Is(TokenKind.RightParen))
			{
				if (current.Is(TokenKind.Eof))
					throw new InvalidOperationException("Invalid eof");

				paramz.Add(Param());
			}

			var closeArg = Next();
			var stmts = new List<StmtAst>();

			if (current.Is(TokenKind.Arrow))
			{
				var arrow = Next();

				stmts.Add(Stmt());
				return new FuncMemAst(ty, name, openArg, paramz.ToArray(), closeArg, arrow, stmts.ToArray());
			}

			var open = Match(TokenKind.LeftBrace);
			while (!current.Is(TokenKind.RightBrace))
			{
				if (current.Is(TokenKind.Eof))
					throw new InvalidOperationException("Finish ur code dude");

				stmts.Add(Stmt());
			}

			var close = Next();
			return new FuncMemAst(ty, name, openArg, paramz.ToArray(), closeArg, open, close, stmts.ToArray());
		}

		private DeclFuncMemAst DeclFuncMem()
		{
			var key = Match(TokenKind.DeclKey);
			var ret = Type();
			var name = Match(TokenKind.Iden);
			var open = Match(TokenKind.LeftParen);
			var paramz = new List<ParamAst>();
			while (!current.Is(TokenKind.RightParen))
			{
				if (current.Is(TokenKind.Eof))
					throw new InvalidOperationException("Invalid eof");

				paramz.Add(Param());
			}

			var close = Next();
			var semi = Match(TokenKind.Semicol);

			return new DeclFuncMemAst(key, ret, name, open, paramz.ToArray(), close, semi);
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

		private ExprAst PreExpr()
		{
			if (current.isPrefix)
				return new PreExprAst(Next(), CallExpr());
			return CallExpr();
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

		private ExprAst CallExpr()
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
			case TokenKind.Str:
			case TokenKind.Iden:
				return LitExpr();
			case TokenKind.LeftParen:
				if (next.isBuiltinType)
					return CastExpr();
				return ParenExpr();

			default:
				throw new InvalidOperationException("Expected expr");
			}
		}

		private LitExprAst LitExpr() =>
			new (Next());

		private CastExprAst CastExpr()
		{
			var open = Match(TokenKind.LeftParen);
			var ty = Type();
			var close = Match(TokenKind.RightParen);

			var value = PreExpr();

			return new (open, ty, close, value);
		}

		private ParenExprAst ParenExpr()
		{
			var open = Match(TokenKind.LeftParen);
			var expr = Expr();
			var close = Match(TokenKind.RightParen);

			return new (open, expr, close);
		}

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
