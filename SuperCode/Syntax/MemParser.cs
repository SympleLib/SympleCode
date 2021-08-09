﻿using System.Collections.Generic;

namespace SuperCode
{
	public partial class Parser
	{
		private MemAst Mem()
		{
			switch (current.kind)
			{
			case TokenKind.PubKey:
			case TokenKind.ProtKey:
			case TokenKind.PrivKey:
				return FuncOrVarMem(Next());
			case TokenKind.MutKey:
			case TokenKind.ConstKey:
				return VarMem(null, Next(), Type());
			case TokenKind.ImportKey:
				return ImportMem();
			case TokenKind.DeclKey:
				return DeclFuncMem();
			case TokenKind.StructKey:
				return StructMem();

			default:
				if (IsType(current))
					return FuncOrVarMem(null);
				return new StmtMemAst(Stmt());
			}
		}

		private DeclFuncMemAst DeclFuncMem()
		{
			var key = Match(TokenKind.DeclKey);
			var ret = Type();
			var name = Match(TokenKind.Iden);
			Token? asmTag = null;
			if (current.kind is TokenKind.Iden or TokenKind.Str)
				asmTag = Next();
			var open = Match(TokenKind.LeftParen);
			var paramz = new List<FieldAst>();

			Token? mutKey = null;
			Token? vaArg = null;
			TypeAst? ty = null;
			while (current.kind is not TokenKind.RightParen and not TokenKind.Eof)
			{
				if (current.kind is TokenKind.DotDotDot)
				{
					vaArg = Next();
					break;
				}

				var field = Field(mutKey, ty);
				ty = field.type;
				mutKey = field.mutKey;
				paramz.Add(field);
			}

			var close = Match(TokenKind.RightParen);
			var semi = Match(TokenKind.Semicol);

			return new DeclFuncMemAst(key, ret, name, asmTag, open, paramz.ToArray(), vaArg, close, semi);
		}

		private ImportMemAst ImportMem()
		{
			var key = Match(TokenKind.ImportKey);
			var what = Match(TokenKind.Str);
			var semi = Match(TokenKind.Semicol);

			return new ImportMemAst(key, what, semi);
		}

		private FuncMemAst FuncMem(Token? vis, TypeAst ret)
		{
			var name = Match(TokenKind.Iden);
			Token? asmTag = null;
			if (current.kind is TokenKind.Iden or TokenKind.Str)
				asmTag = Next();
			var openArg = Match(TokenKind.LeftParen);
			var paramz = new List<FieldAst>();

			Token? mutKey = null;
			Token? vaArg = null;
			TypeAst? ty = null;
			while (current.kind is not TokenKind.RightParen and not TokenKind.Eof)
			{
				if (current.kind is TokenKind.DotDotDot)
				{
					vaArg = Next();
					break;
				}

				var field = Field(mutKey, ty);
				ty = field.type;
				mutKey = field.mutKey;
				paramz.Add(field);
			}

			var closeArg = Match(TokenKind.RightParen);
			var stmts = new List<StmtAst>();

			if (current.kind is TokenKind.Arrow)
			{
				var arrow = Next();

				stmts.Add(Stmt());
				return new FuncMemAst(vis, ret, name, asmTag, openArg, paramz.ToArray(), vaArg, closeArg, arrow, stmts.ToArray());
			}

			var open = Match(TokenKind.LeftBrace);
			while (current.kind is not TokenKind.RightBrace and not TokenKind.Eof)
				stmts.Add(Stmt());

			var close = Match(TokenKind.RightBrace);
			return new FuncMemAst(vis, ret, name, asmTag, openArg, paramz.ToArray(), vaArg, closeArg, open, close, stmts.ToArray());
		}

		private MemAst FuncOrVarMem(Token? vis)
		{
			var type = Type();
			if (next.kind is TokenKind.LeftParen || Peek(2).kind is TokenKind.LeftParen)
				return FuncMem(vis, type);
			Token? mutKey = null;
			if (current.kind is TokenKind.MutKey or TokenKind.ConstKey)
				mutKey = Next();
			return VarMem(vis, mutKey, type);
		}

		private StructMemAst StructMem()
		{
			var key = Match(TokenKind.StructKey);
			var name = Match(TokenKind.Iden);
			var open = Match(TokenKind.LeftBrace);
			var fields = new List<FieldAst>();

			Token? mutKey = null;
			TypeAst? ty = null;
			while (current.kind is not TokenKind.RightBrace and not TokenKind.Eof)
			{
				var field = Field(mutKey, ty);
				ty = field.type;
				mutKey = field.mutKey;
				fields.Add(field);
			}
			var close = Match(TokenKind.RightBrace);

			types.Add(name.text);
			return new StructMemAst(key, name, open, fields.ToArray(), close);
		}


		private VarMemAst VarMem(Token? vis, Token? mutKey, TypeAst type)
		{
			var name = Match(TokenKind.Iden);
			if (current.kind is TokenKind.Semicol)
			{
				var semi = Next();
				return new VarMemAst(vis, mutKey, type, name, null, null, semi);
			}

			var eql = Match(TokenKind.Eql);
			var init = Expr();
			var semicol = Match(TokenKind.Semicol);
			return new VarMemAst(vis, mutKey, type, name, eql, init, semicol);
		}
	}
}