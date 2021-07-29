using System;
using System.Collections.Generic;

namespace SuperCode
{
	public class Parser
	{
		public readonly PermaSafe safety = new PermaSafe();
		public readonly Lexer lexer;
		public readonly Token[] tokens;
		public readonly List<string> types = new List<string>();
		private int pos;

		private Token current => pos < tokens.Length ? tokens[pos] : default;
		private Token next => pos + 1 < tokens.Length ? tokens[pos + 1] : default;
		
		public Parser(string file)
		{
			lexer = new Lexer(file);
			tokens = lexer.Lex();
		}

		public PermaSafe Parse(out ModuleAst module)
		{
			var mems = new List<MemAst>();
			while (current.isComment)
				Next();
			while (!current.Is(TokenKind.Eof))
				mems.Add(Mem());

			var eof = Next();
			module = new ModuleAst(lexer.path, mems.ToArray(), eof);
			return safety;
		}


		private ExprAst[] Args(Token open)
		{
			var args = new List<ExprAst>();
			while (!current.Is(TokenKind.RightParen))
			{
				if (current.Is(TokenKind.Eof))
				{
					safety.ReportUnexpectedEof(open);
					return null;
				}

				args.Add(Expr());
				if (current.Is(TokenKind.Comma))
					Next();
			}

			return args.ToArray();
		}

		private TypeAst Type()
		{
			var baze = Next();
			var addons = new List<Token>();
			while (current.isTypeAddon)
				addons.Add(Next());

			return new TypeAst(baze, addons.ToArray());
		}

		private FieldAst Field(TypeAst ty = null)
		{
			if (ty is null || IsType(current))
				ty = Type();
			
			var name = current.Is(TokenKind.Iden) ? Next() : default;
			Token comma = default;
			if (current.Is(TokenKind.Eql))
			{
				var eql = Next();
				var defVal = Expr();

				if (current.Is(TokenKind.Comma))
					comma = Next();

				return new FieldAst(ty, name, eql, defVal, comma);
			}

			if (current.Is(TokenKind.Comma))
				comma = Next();

			return new FieldAst(ty, name, comma);
		}
		

		private MemAst Mem()
		{
			switch (current.kind)
			{
			case TokenKind.DeclKey:
				return DeclFuncMem();
			case TokenKind.StructKey:
				return StructMem();

			default:
				if (IsType(current))
					return FuncOrVarMem();
				return new StmtMemAst(Stmt());
			}
		}

		private DeclFuncMemAst DeclFuncMem()
		{
			var key = Match(TokenKind.DeclKey);
			var ret = Type();
			var name = Match(TokenKind.Iden);
			Token asmTag = default;
			if (current.Is(TokenKind.Iden, TokenKind.Str))
				asmTag = Next();
			var open = Match(TokenKind.LeftParen);
			var paramz = new List<FieldAst>();

			TypeAst ty = null;
			while (!current.Is(TokenKind.RightParen))
			{
				if (current.Is(TokenKind.Eof))
				{
					safety.ReportUnexpectedEof(open);
					return null;
				}

				var field = Field(ty);
				ty = field.type;
				paramz.Add(field);
			}

			var close = Next();
			var semi = Match(TokenKind.Semicol);

			return new DeclFuncMemAst(key, ret, name, asmTag, open, paramz.ToArray(), close, semi);
		}

		private FuncMemAst FuncMem()
		{
			var ret = Type();
			var name = Match(TokenKind.Iden);
			Token asmTag = default;
			if (current.Is(TokenKind.Iden, TokenKind.Str))
				asmTag = Next();
			var openArg = Match(TokenKind.LeftParen);
			var paramz = new List<FieldAst>();

			TypeAst ty = null;
			while (!current.Is(TokenKind.RightParen))
			{
				if (current.Is(TokenKind.Eof))
				{
					safety.ReportUnexpectedEof(openArg);
					return null;
				}


				var field = Field(ty);
				ty = field.type;
				paramz.Add(field);
			}

			var closeArg = Next();
			var stmts = new List<StmtAst>();

			if (current.Is(TokenKind.Arrow))
			{
				var arrow = Next();

				stmts.Add(Stmt());
				return new FuncMemAst(ret, name, asmTag, openArg, paramz.ToArray(), closeArg, arrow, stmts.ToArray());
			}

			var open = Match(TokenKind.LeftBrace);
			while (!current.Is(TokenKind.RightBrace))
			{
				if (current.Is(TokenKind.Eof))
				{
					safety.ReportUnexpectedEof(open);
					return null;
				}

				stmts.Add(Stmt());
			}

			var close = Next();
			return new FuncMemAst(ret, name, asmTag, openArg, paramz.ToArray(), closeArg, open, close, stmts.ToArray());
		}

		// TODO: stuff
		private MemAst FuncOrVarMem()
		{
			if (Peek(2).Is(TokenKind.LeftParen) || Peek(3).Is(TokenKind.LeftParen))
				return FuncMem();
			return VarMem();
		}

		private StructMemAst StructMem()
		{
			var key = Match(TokenKind.StructKey);
			var name = Match(TokenKind.Iden);
			var open = Match(TokenKind.LeftBrace);
			var fields = new List<FieldAst>();

			TypeAst ty = null;
			while (!current.Is(TokenKind.RightBrace))
			{
				if (current.Is(TokenKind.Eof))
				{
					safety.ReportUnexpectedEof(open);
					return null;
				}


				var field = Field(ty);
				ty = field.type;
				fields.Add(field);
			}
			var close = Next();

			types.Add(name.text);
			return new StructMemAst(key, name, open, fields.ToArray(), close);
		}


		private VarMemAst VarMem()
		{
			var type = Type();
			var name = Match(TokenKind.Iden);
			if (current.Is(TokenKind.Semicol))
			{
				var semi = Next();
				return new VarMemAst(type, name, default, default, semi);
			}

			var eql = Match(TokenKind.Eql);
			var init = Expr();
			var semicol = Match(TokenKind.Semicol);
			return new VarMemAst(type, name, eql, init, semicol);
		}

		private StmtAst Stmt()
		{
			switch (current.kind)
			{
			case TokenKind.UsingKey:
				return UsingStmt();
			case TokenKind.RetKey:
				return RetStmt();
			case TokenKind.IfKey:
				return IfStmt();

			default:
				if (current.isBuiltinType || types.Contains(current.text))
					return VarStmt();
				return ExprStmt();
			}
		}

		private IfStmtAst IfStmt()
		{
			var ifKey = Match(TokenKind.IfKey);
			var cond = Expr();

			if (current.Is(TokenKind.Arrow))
			{
				var arrow = Next();
				var then = Stmt();

				if (current.Is(TokenKind.ElseKey))
				{
					var elseKey = Next();
					var elze = Stmt();

					return new IfStmtAst(ifKey, cond, arrow, then, elseKey, elze);
				}

				return new IfStmtAst(ifKey, cond, arrow, then, default, default);
			}

			var open = Match(TokenKind.LeftBrace);
			var stmts = new List<StmtAst>();
			while (!current.Is(TokenKind.RightBrace))
			{
				if (current.Is(TokenKind.Eof))
				{
					safety.ReportUnexpectedEof(current);
					break;
				}

				stmts.Add(Stmt());
			}
			var close = Next();

			if (current.Is(TokenKind.ElseKey))
			{
				var elseKey = Next();
				var elze = Stmt();
				return new IfStmtAst(ifKey, cond, open, stmts.ToArray(), close, elseKey, elze);
			}

			return new IfStmtAst(ifKey, cond, open, stmts.ToArray(), close, default, default);
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
			if (ty is null || current.isBuiltinType || types.Contains(current.text))
				ty = Type();
			var name = Match(TokenKind.Iden);

			if (current.Is(TokenKind.Eql))
			{
				var eql = Next();
				var init = Expr();

				return new VarStmtAst(ty, name, eql, init, Match(TokenKind.Semicol));
			}

			return new VarStmtAst(ty, name, default, default, Match(TokenKind.Semicol));
		}

		private UsingStmtAst UsingStmt()
		{
			var key = Match(TokenKind.UsingKey);
			var real = Type();
			var asKey = Match(TokenKind.AsKey);
			var alias = Match(TokenKind.Iden);
			var semi = Match(TokenKind.Semicol);
			types.Add(alias.text);
			return new UsingStmtAst(key, real, asKey, alias, semi);
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

		private CastExprAst CastExpr()
		{
			var open = Match(TokenKind.LeftParen);
			var ty = Type();
			var close = Match(TokenKind.RightParen);

			var expr = PreExpr();

			return new (open, ty, close, expr);
		}

		private ExprAst CallExpr()
		{
			var expr = PrimExpr();
			while (current.Is(TokenKind.LeftParen))
			{
				var open = Next();
				var args = Args(open);
				var close = Match(TokenKind.RightParen);
				expr = new CallExprAst(expr, open, args, close);
			}

			return expr;
		}

		private LitExprAst LitExpr() =>
			new LitExprAst(Next());

		private ParenExprAst ParenExpr()
		{
			var open = Match(TokenKind.LeftParen);
			var expr = Expr();
			var close = Match(TokenKind.RightParen);

			return new (open, expr, close);
		}

		private ExprAst PreExpr()
		{
			if (current.isPrefix)
				return new PreExprAst(Next(), CallExpr());
			return CallExpr();
		}

		private ExprAst PrimExpr()
		{
			switch (current.kind)
			{
			case TokenKind.LeftParen:
				if (next.isBuiltinType)
					return CastExpr();
				return ParenExpr();
			case TokenKind.Num:
			case TokenKind.Str:
			case TokenKind.Iden:
			case TokenKind.NullKey:
				return LitExpr();
			case TokenKind.LeftBracket:
				return TypePun();

			default:
				throw new InvalidOperationException("Expected expr");
			}
		}

		private TypePunExprAst TypePun()
		{
			var open = Match(TokenKind.LeftBracket);
			var ty = Type();
			var close = Match(TokenKind.RightBracket);

			var expr = PreExpr();

			return new (open, ty, close, expr);
		}


		private bool IsType(Token tok) =>
			tok.Is(TokenKind.Iden) && (tok.isBuiltinType || types.Contains(tok.text));

		private Token Peek(int off)
		{
			int i = pos + off;
			if (i < 0)
				return tokens[0];
			if (i >= tokens.Length)
				return tokens[^1];
			return tokens[i];
		}

		private Token Next()
		{
			Token tok = current;
			pos++;
			while (current.isComment)
				pos++;
			return tok;
		}

		private Token Match(TokenKind kind)
		{
			if (current.Is(kind))
				return Next();

			safety.ReportExpectedToken(kind, current);
			return current;
		}
	}
}
