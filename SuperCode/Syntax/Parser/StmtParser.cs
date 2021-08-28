﻿using System.Collections.Generic;

namespace SuperCode
{
	public partial class Parser
	{
		private StmtAst Stmt()
		{
			switch (current.kind)
			{
			case TokenKind.MutKey:
			case TokenKind.ConstKey:
				return VarStmt();
			case TokenKind.LeftBrace:
				return BlockStmt();
			case TokenKind.UsingKey:
				return UsingStmt();
			case TokenKind.RetKey:
				return RetStmt();
			case TokenKind.IfKey:
				return IfStmt();
			case TokenKind.WhileKey:
				return WhileStmt();
			case TokenKind.AsmKey:
				return AsmStmt();

			default:
				if (next.kind is TokenKind.Iden)
					return VarStmt();
				return ExprStmt();
			}
		}

		private AsmStmtAst AsmStmt()
		{
			var key = Match(TokenKind.AsmKey);
			var asm = Match(TokenKind.Str);
			var semi = Match(TokenKind.Semicol);

			return new AsmStmtAst(key, asm, semi);
		}

		private BlockStmtAst BlockStmt()
		{
			var open = Match(TokenKind.LeftBrace);
			var stmts = new List<StmtAst>();
			while (current.kind is not TokenKind.RightBrace and not TokenKind.Eof)
				stmts.Add(Stmt());
			var close = Match(TokenKind.RightBrace);

			return new BlockStmtAst(open, stmts.ToArray(), close);
		}

		private IfStmtAst IfStmt()
		{
			scope = new ParseScope(scope);
			var ifKey = Match(TokenKind.IfKey);
			var cond = Expr();

			if (current.kind is TokenKind.Arrow)
			{
				var arrow = Next();
				var then = Stmt();

				if (current.kind is TokenKind.ElseKey)
				{
					var elseKey = Next();
					var elze = Stmt();

					return new IfStmtAst(ifKey, cond, arrow, then, elseKey, elze);
				}

				return new IfStmtAst(ifKey, cond, arrow, then, null, null);
			}

			var open = Match(TokenKind.LeftBrace);
			var stmts = new List<StmtAst>();
			while (current.kind is not TokenKind.RightBrace and not TokenKind.Eof)
				stmts.Add(Stmt());
			var close = Match(TokenKind.RightBrace);

			scope = scope.parent!;
			if (current.kind is TokenKind.ElseKey)
			{
				scope = new ParseScope();
				var elseKey = Next();
				var elze = Stmt();
				scope = scope.parent!;
				return new IfStmtAst(ifKey, cond, open, stmts.ToArray(), close, elseKey, elze);
			}

			return new IfStmtAst(ifKey, cond, open, stmts.ToArray(), close, default, default);
		}

		private RetStmtAst RetStmt()
		{
			var key = Match(TokenKind.RetKey);
			ExprAst? val = null;
			if (current.kind is not TokenKind.Semicol)
				val = Expr();
			var semi = Match(TokenKind.Semicol);

			return new RetStmtAst(key, val, semi);
		}

		private UsingStmtAst UsingStmt()
		{
			var key = Match(TokenKind.UsingKey);
			var real = Type();
			var asKey = Match(TokenKind.AsKey);
			var alias = Match(TokenKind.Iden);
			var semi = Match(TokenKind.Semicol);

			return new UsingStmtAst(key, real, asKey, alias, semi);
		}

		private VarStmtAst VarStmt(Token? mutKey = null, TypeAst? ty = null)
		{
			if (current.kind is TokenKind.MutKey or TokenKind.ConstKey)
				mutKey = Next();
			if (ty is null || next.kind is TokenKind.Iden)
				ty = Type();
			var name = Match(TokenKind.Iden);
			scope.vars.Add(name.text);

			if (current.kind is TokenKind.Eql)
			{
				var eql = Next();
				var init = Expr();

				return new VarStmtAst(mutKey, ty, name, eql, init, Match(TokenKind.Semicol));
			}

			return new VarStmtAst(mutKey, ty, name, null, null, Match(TokenKind.Semicol));
		}

		private WhileStmtAst WhileStmt()
		{
			scope = new ParseScope();
			var key = Match(TokenKind.WhileKey);
			var cond = Expr();
			if (current.kind is TokenKind.Arrow)
			{
				var arrow = Next();
				var stmt = Stmt();
				return new WhileStmtAst(key, cond, arrow, stmt);
			}

			var open = Match(TokenKind.LeftBrace);
			var stmts = new List<StmtAst>();
			while (current.kind is not TokenKind.RightBrace and not TokenKind.Eof)
				stmts.Add(Stmt());

			var close = Match(TokenKind.RightBrace);
			scope = scope.parent!;
			return new WhileStmtAst(key, cond, open, stmts.ToArray(), close);
		}

		private ExprStmtAst ExprStmt() =>
			new ExprStmtAst(Expr(), Match(TokenKind.Semicol));
	}
}