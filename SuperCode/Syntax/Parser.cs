using System;
using System.Collections.Generic;

namespace SuperCode
{
	public partial class Parser
	{
		public readonly PermaSafe safety = new PermaSafe();
		public readonly Lexer lexer;
		public readonly Token[] tokens;
		private ParseStep step = ParseStep.Imports;
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

			// Cheatcode
			var implSyms = new List<MemAst>();
			var implStructs = new List<MemAst>();
			foreach (var mem in mems)
				switch (mem.kind)
				{
				case AstKind.VarMem:
				{
					var var = (VarMemAst) mem;
					implSyms.Add(new ImplVarMemAst(var.mutKey, var.type, var.name));
					break;
				}
				case AstKind.FuncMem:
				{
					var func = (FuncMemAst) mem;
					implSyms.Add(new ImplFuncMemAst(func.retType, func.name, func.asmTag, func.paramz, func.vaArg));
					break;
				}
				case AstKind.StructMem:
				{
					var ztruct = (StructMemAst) mem;
					implStructs.Add(new ImplStructMemAst(ztruct.name));
					break;
				}

				default:
					break;
				}

			int insert = mems.FindIndex(mem => mem.kind is AstKind.StructMem);
			mems.InsertRange(insert == -1 ? 0 : insert, implStructs);
			insert = mems.FindIndex(mem => mem.kind is AstKind.FuncMem);
			mems.InsertRange(insert == -1 ? 0 : insert, implSyms);

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
			if (ty is null || (next.kind is TokenKind.Iden or TokenKind.Eql or TokenKind.Comma))
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


		private bool HasUntil(TokenKind stop, params TokenKind[] find)
		{
			for (int i = pos; i < tokens.Length; i++)
			{
				ref var token = ref tokens[i];
				if (token.kind == stop)
					return false;

				foreach (TokenKind kind in find)
					if (token.kind == kind)
						return true;
			}

			return false;
		}

		private int Find(TokenKind kind)
		{
			for (int i = pos; i < tokens.Length; i++)
				if (tokens[i].kind == kind)
					return i;
			return -1;
		}

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
