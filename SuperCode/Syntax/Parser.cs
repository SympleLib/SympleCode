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
			while (current.kind is not TokenKind.Eof)
				mems.Add(Mem());

			var eof = Next();
			module = new ModuleAst(lexer.path, mems.ToArray(), eof);
			return safety;
		}


		private ExprAst[] Args()
		{
			var args = new List<ExprAst>();
			while (current.kind is not TokenKind.RightParen and not TokenKind.Eof)
			{
				args.Add(Expr());
				if (current.kind is TokenKind.Comma)
					Next();
			}

			return args.ToArray();
		}

		private ElementAst Element()
		{
			var val = Expr();
			var comma = Match(TokenKind.Comma);
			return new ElementAst(val, comma);
		}

		private TypeAst Type()
		{
			var baze = Next();
			var args = new List<TypeAst>();
			Token? open, close;
			open = close = null;
			if (current.kind is TokenKind.LeftParen)
			{
				open = Next();
				while (current.kind is not TokenKind.RightParen and not TokenKind.Eof)
					args.Add(Type());
				close = Match(TokenKind.RightParen);
			}

			var addons = new List<Token>();
			while (current.isTypeAddon)
				addons.Add(Next());
			Token? refTok = null;
			if (current.kind is TokenKind.RefKey)
				refTok = Next();

			return new TypeAst(baze, open, args.ToArray(), close, addons.ToArray(), refTok);
		}

		private FieldAst Field(Token? mutKey = null, TypeAst? ty = null)
		{
			if (current.kind is TokenKind.MutKey or TokenKind.ConstKey)
				mutKey = Next();
			if (ty is null || IsType(current))
				ty = Type();
			
			var name = current.kind is TokenKind.Iden ? Next() : default;
			Token comma = default;
			if (current.kind is TokenKind.Eql)
			{
				var eql = Next();
				var defVal = Expr();

				if (current.kind is TokenKind.Comma)
					comma = Next();

				return new FieldAst(mutKey, ty, name, eql, defVal, comma);
			}

			if (current.kind is not TokenKind.RightBrace and not TokenKind.RightBracket and not TokenKind.RightParen)
				comma = Match(TokenKind.Comma);

			return new FieldAst(mutKey, ty, name, comma);
		}
		

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

			default:
				if (current.isBuiltinType || types.Contains(current.text))
					return VarStmt();
				return ExprStmt();
			}
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

			if (current.kind is TokenKind.ElseKey)
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
			types.Add(alias.text);
			return new UsingStmtAst(key, real, asKey, alias, semi);
		}

		private VarStmtAst VarStmt(Token? mutKey = null, TypeAst? ty = null)
		{
			if (current.kind is TokenKind.MutKey or TokenKind.ConstKey)
				mutKey = Next();
			if (ty is null || current.isBuiltinType || types.Contains(current.text))
				ty = Type();
			var name = Match(TokenKind.Iden);

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
			return new WhileStmtAst(key, cond, open, stmts.ToArray(), close);
		}

		private ExprStmtAst ExprStmt() =>
			new ExprStmtAst(Expr(), Match(TokenKind.Semicol));


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
				if (IsType(next))
					return CastExpr();
				return ParenExpr();
			case TokenKind.Num:
			case TokenKind.Str:
			case TokenKind.Iden:
			case TokenKind.NullKey:
				return LitExpr();
			case TokenKind.LeftBracket:
				if (IsType(next))
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


		private bool IsType(Token tok) =>
			tok.kind is TokenKind.Iden && (tok.isBuiltinType || types.Contains(tok.text));

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

		private Token Match(TokenKind kind, params TokenKind[] kinds)
		{
			if (current.kind == kind)
				return Next();
			foreach (var okind in kinds)
				if (current.kind == okind)
					return Next();

			safety.ReportExpectedToken(kind, current);
			return current;
		}
	}
}
