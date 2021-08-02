using System;
using System.Collections.Generic;

using LLVMSharp.Interop;

namespace SuperCode
{
	public class Noder
	{
		public readonly PermaSafe safety = new PermaSafe();
		public readonly ModuleAst module;
		public readonly LLVMContextRef ctx;

		private Dictionary<string, Symbol> syms = new Dictionary<string, Symbol>();
		private readonly Dictionary<string, LLVMTypeRef> types = new Dictionary<string, LLVMTypeRef>();
		private readonly Dictionary<LLVMTypeRef, StructMemNode> ztructs = new Dictionary<LLVMTypeRef, StructMemNode>();
		private readonly SympleCode syc;
		private LLVMTypeRef retType;

		public Noder(SympleCode syc, ModuleAst module)
		{
			this.syc = syc;
			this.module = module;
			ctx = LLVMContextRef.Global;
		}

		public PermaSafe Nodify(out ModuleNode node)
		{
			var mems = new Node[module.mems.Length];
			for (int i = 0; i < mems.Length; i++)
				mems[i] = Nodify(module.mems[i]);
			node = new ModuleNode(module.filename, mems) { syntax = module };
			return safety;
		}

		private LLVMTypeRef Nodify(TypeAst ast)
		{
			LLVMTypeRef ty;
			if (ast.baze.isBuiltinType)
				ty = ast.baze.builtinType;
			else
				ty = types[ast.baze.text];

			if (ast.args.Length > 0)
			{
				var args = new LLVMTypeRef[ast.args.Length];
				for (int i = 0; i < args.Length; i++)
					args[i] = Nodify(ast.args[i]);
				ty = LLVMTypeRef.CreateFunction(ty, args);
			}
			
			if (ast.addons.Length > 0 && !syc.allUnsafe)
					safety.ReportEvilPtr(ast);
			for (int i = 0; i < ast.addons.Length; i++)
				ty = ty.Ptr();
			if (ast.refTok is not null)
				ty = ty.Ref();
			return ty;
		}

		private FieldNode Nodify(FieldAst ast, int index) =>
			new FieldNode(Nodify(ast.type), IsMut(ast.mutKey), index, ast.name.text) { syntax = ast };

		private Node Nodify(MemAst ast)
		{
			switch (ast.kind)
			{
			case AstKind.DeclFuncMem:
				return Nodify((DeclFuncMemAst) ast);
			case AstKind.FuncMem:
				return Nodify((FuncMemAst) ast);
			case AstKind.StmtMem:
				return Nodify((StmtMemAst) ast);
			case AstKind.StructMem:
				return Nodify((StructMemAst) ast);
			case AstKind.VarMem:
				return Nodify((VarMemAst) ast);

			default:
				throw new InvalidOperationException("Invalid mem");
			}
		}

		private DeclFuncMemNode Nodify(DeclFuncMemAst ast)
		{
			var paramTypes = new LLVMTypeRef[ast.paramz.Length];
			var paramz = new FieldNode[ast.paramz.Length];
			for (int i = 0; i < paramz.Length; i++)
			{
				var param = Nodify(ast.paramz[i], i);
				paramz[i] = param;
				paramTypes[i] = param.type;
			}

			retType = Nodify(ast.retType);
			var ty = LLVMTypeRef.CreateFunction(retType, paramTypes, ast.vaArg != default);
			string name = ast.name.text;
			if (ast.asmTag is not null)
			{
				var asmTag = ast.asmTag.Value;
				name = asmTag.text;
				if (asmTag.Is(TokenKind.Str))
					name = name[1..^1];
			}

			var decl = new DeclFuncMemNode(ty, name, paramz) { syntax = ast };
			syms.Add(ast.name.text, decl);
			return decl;
		}

		private FuncMemNode Nodify(FuncMemAst ast)
		{
			var psyms = new Dictionary<string, Symbol>(syms);

			var paramTypes = new LLVMTypeRef[ast.paramz.Length];
			for (int i = 0; i < paramTypes.Length; i++)
				paramTypes[i] = Nodify(ast.paramz[i].type);

			var paramz = new FieldNode[ast.paramz.Length];
			for (int i = 0; i < paramz.Length; i++)
			{
				var paramNode = Nodify(ast.paramz[i], i);
				paramz[i] = paramNode;
				syms.Add(paramNode.name, paramNode);
			}

			retType = Nodify(ast.retType);
			var ty = LLVMTypeRef.CreateFunction(retType, paramTypes, ast.vaArg != default);
			string name = ast.name.text;
			if (ast.asmTag is not null)
			{
				var asmTag = ast.asmTag.Value;
				name = asmTag.text;
				if (asmTag.Is(TokenKind.Str))
					name = name[1..^1];
			}

			var stmts = new Node[ast.stmts.Length];
			for (int i = 0; i < stmts.Length; i++)
				stmts[i] = Nodify(ast.stmts[i]);

			var func = new FuncMemNode(ty, Vis(ast.vis), name, paramz, stmts) { syntax = ast };
			syms = psyms;
			if (syms.ContainsKey(ast.name.text))
			{
				syms.Remove(ast.name.text, out var sym);
				((DeclFuncMemNode) sym).impl = func;
			}
			syms.Add(ast.name.text, func);
			return func;
		}

		private Node Nodify(StmtMemAst ast) =>
			Nodify(ast.stmt);

		private StructMemNode Nodify(StructMemAst ast)
		{
			var fields = new FieldNode[ast.fields.Length];
			var fieldTypes = new LLVMTypeRef[ast.fields.Length];
			for (int i = 0; i < fields.Length; i++)
			{
				var field = Nodify(ast.fields[i], i);
				fields[i] = field;
				fieldTypes[i] = field.type;
			}

			string name = ast.name.text;
			var type = ctx.CreateNamedStruct(name);
			type.StructSetBody(fieldTypes, true);
			var node = new StructMemNode(type, name, fields);
			ztructs.Add(type, node);
			types.Add(name, type);
			return node;
		}

		private VarMemNode Nodify(VarMemAst ast)
		{
			var ty = Nodify(ast.type);
			var init = Nodify(ast.init, ty.IsRef() ? default : ty);
			var var = new VarMemNode(ty, Vis(ast.vis), IsMut(ast.mutKey), ast.name.text, init) { syntax = ast };
			syms.Add(var.name, var);
			return var;
		}


		private Node Nodify(StmtAst ast)
		{
			switch (ast.kind)
			{
			case AstKind.BlockStmt:
				return Nodify(((BlockStmtAst) ast));
			case AstKind.ExprStmt:
				return Nodify(((ExprStmtAst) ast).expr);
			case AstKind.IfStmt:
				return Nodify(((IfStmtAst) ast));
			case AstKind.RetStmt:
				return Nodify((RetStmtAst) ast);
			case AstKind.VarStmt:
				return Nodify((VarStmtAst) ast);
			case AstKind.UsingStmt:
				return Nodify((UsingStmtAst) ast);

			default:
				throw new InvalidOperationException("Invalid stmt");
			}
		}

		private BlockStmtNode Nodify(BlockStmtAst ast)
		{
			var psyms = new Dictionary<string, Symbol>(syms);

			var stmts = new Node[ast.stmts.Length];
			for (int i = 0; i < stmts.Length; i++)
				stmts[i] = Nodify(ast.stmts[i]);

			syms = psyms;
			return new BlockStmtNode(stmts);
		}

		private IfStmtNode Nodify(IfStmtAst ast)
		{
			var cond = Nodify(ast.cond);
			var stmts = new Node[ast.then.Length];
			for (int i = 0; i < stmts.Length; i++)
				stmts[i] = Nodify(ast.then[i]);
			var then = new BlockStmtNode(stmts);
			Node elze = null;
			if (ast.elze is not null)
				elze = Nodify(ast.elze);

			return new IfStmtNode(cond, then, elze);
		}

		private RetStmtNode Nodify(RetStmtAst ast)
		{

			if (retType == LLVMTypeRef.Void)
			{
				if (ast.value is not null)
					throw new Exception("Must return null");
				return new RetStmtNode(null);
			}

			return new RetStmtNode(Nodify(ast.value, retType));
		}

		private VarStmtNode Nodify(VarStmtAst ast)
		{
			var ty = Nodify(ast.type);
			var initTy = ty.IsRef() ? default : ty;
			var init = Nodify(ast.init, initTy);
			var var = new VarStmtNode(ty, IsMut(ast.mutKey), ast.name.text, init) { syntax = ast };
			syms.Add(var.name, var);
			return var;
		}

		private TypedefStmtNode Nodify(UsingStmtAst ast)
		{
			string alias = ast.alias.text;
			var real = Nodify(ast.real);
			types.Add(alias, real);
			return new TypedefStmtNode(alias, real);
		}

		
		private ExprNode Nodify(ExprAst ast, LLVMTypeRef castTo = default)
		{
			if (ast is null)
				return Cast(new NumExprNode(0, LLVMTypeRef.Int1), castTo);

			switch (ast.kind)
			{
			case AstKind.BinExpr:
				return Cast(Nodify((BinExprAst) ast), castTo);
			case AstKind.CallExpr:
				return Cast(Nodify((CallExprAst) ast), castTo);
			case AstKind.CastExpr:
				return Cast(Nodify((CastExprAst) ast), castTo);
			case AstKind.IndexExpr:
				return Cast(Nodify((IndexExprAst) ast), castTo);
			case AstKind.LitExpr:
				return Cast(Nodify((LitExprAst) ast), castTo);
			case AstKind.ParenExpr:
				return Cast(Nodify(((ParenExprAst) ast).expr), castTo);
			case AstKind.PreExpr:
				return Cast(Nodify((PreExprAst) ast), castTo);
			case AstKind.TypePunExpr:
				return Cast(Nodify((TypePunExprAst) ast), castTo);

			default:
				throw new InvalidOperationException("Invalid expr");
			}
		}

		private ExprNode Nodify(BinExprAst ast)
		{
			var left = Nodify(ast.left);
			switch (ast.op.kind)
			{
			case TokenKind.Dot:
			{
				var name = ast.right.token;
				var field = Mem(left.type, name);
				return new MemExprNode(left, field);
			}
			case TokenKind.Eql:
			{
				if (!left.mut)
					safety.ReportNotMut(left);
				var val = Nodify(ast.right);
				return new AssignExprNode(left, Cast(val, left.type.IsRef() ? left.type.ElementType : left.type));
			}
			}

			var right = Nodify(ast.right, left.type);
			bool fp = left.type.IsFloat();
			bool us = left.type.IsUnsigned();
			
			BinOp op;
			switch (ast.op.kind)
			{
			case TokenKind.EqlEql:
				op = fp ? BinOp.FEql : BinOp.Eql;
				goto LogExpr;
			case TokenKind.NotEql:
				op = fp ? BinOp.FNEql : BinOp.NEql;
				goto LogExpr;
			case TokenKind.LeftChevron:
				op = fp ? BinOp.FLt : us ? BinOp.ULt : BinOp.SLt;
				goto LogExpr;
			case TokenKind.RightChevron:
				op = fp ? BinOp.FGt : us ? BinOp.UGt : BinOp.SGt;
				goto LogExpr;
			case TokenKind.LEql:
				op = fp ? BinOp.FLtEql : us ? BinOp.ULtEql : BinOp.SLtEql;
				goto LogExpr;
			case TokenKind.REql:
				op = fp ? BinOp.FGtEql : us ? BinOp.UGtEql : BinOp.SGtEql;
				goto LogExpr;

			case TokenKind.Plus:
				op = fp ? BinOp.FAdd : BinOp.Add;
				goto BinExpr;
			case TokenKind.Dash:
				op = fp ? BinOp.FSub : BinOp.Sub;
				goto BinExpr;
			case TokenKind.Star:
				op = fp ? BinOp.FMul : BinOp.Mul;
				goto BinExpr;
			case TokenKind.Slash:
				op = fp ? BinOp.FDiv : us ? BinOp.UDiv : BinOp.SDiv;
				goto BinExpr;
			case TokenKind.Percent:
				op = fp ? BinOp.FMod : us ? BinOp.UMod : BinOp.SMod;
				goto BinExpr;

			default:
				throw new InvalidOperationException("Invalid bin-expr");
			}

		LogExpr:
			return new BinExprNode(op, left, right, LLVMTypeRef.Int1);
		BinExpr:
			return new BinExprNode(op, left, right, left.type) { syntax = ast };
		}

		private CallExprNode Nodify(CallExprAst ast)
		{
			var what = Nodify(ast.what);
			var paramTypes = what.type.IsPtr() ? what.type.ElementType.ParamTypes : what.type.ParamTypes;

			var args = new ExprNode[ast.args.Length];
			for (int i = 0; i < args.Length; i++)
			{
				LLVMTypeRef type = default;
				if (i < paramTypes.Length)
					type = paramTypes[i];
				args[i] = Nodify(ast.args[i], type);
			}

			return new CallExprNode(what, args) { syntax = ast };
		}

		private ExprNode Nodify(CastExprAst ast)
		{
			var node = Nodify(ast.expr);
			var to = Nodify(ast.type);
			if (to == default || node.type == to)
				return node;
			return new CastExprNode(node, to) { syntax = ast };
		}

		private BinExprNode Nodify(IndexExprAst ast)
		{
			var expr = Nodify(ast.expr);
			var index = Nodify(ast.index);
			var type = expr.type.ElementType;

			return new BinExprNode(BinOp.Index, expr, index, type);
		}

		private ExprNode Nodify(LitExprAst ast)
		{
			string literal = ast.literal.text;
			switch (ast.literal.kind)
			{
			case TokenKind.NullKey:
				// Chez
				return new NumExprNode(0, LLVMTypeRef.Int64) { syntax = ast };
			case TokenKind.Iden:
				if (syms.TryGetValue(literal, out var sym))
					return new SymExprNode(sym) { syntax = ast };
				safety.ReportUS(ast.literal);
				return new SymExprNode(new BadSymbol()) { syntax = ast };
			case TokenKind.Str:
			{
				int start = literal[0] == '\'' ? 1 : 2;
				var type = literal[0] switch
				{
					's' => BuiltinTypes.types["schar"].Ptr(),
					'u' => BuiltinTypes.types["char"].Ptr(),
					'w' => BuiltinTypes.types["wchar"].Ptr(),

					_ => BuiltinTypes.types["char"].Ptr(),
				};

				return new StrExprNode(type, literal[start..^1]) { syntax = ast };
			}
			case TokenKind.Num:
				if (literal.Contains('.'))
				{
					bool isF32 = literal.Contains('f') || literal.Contains('F');
					double num = double.Parse(isF32 ? literal[..^1] : literal);
					var ty = isF32 ? LLVMTypeRef.Float : LLVMTypeRef.Double;
					return new FNumExprNode(num, ty) { syntax = ast };
				}
				else
				{
					ulong num = ulong.Parse(literal);
					var ty = num != (uint) num ? LLVMTypeRef.Int64 : LLVMTypeRef.Int32;
					return new NumExprNode(num, ty) { syntax = ast };
				}

			default:
				throw new InvalidOperationException("Invalid lit-expr");
			}
		}

		private ExprNode Nodify(PreExprAst ast)
		{
			if (ast.prefix.Is(TokenKind.Plus))
				return Nodify(ast.expr);

			UnOp op;
			var var = Nodify(ast.expr);
			var ty = var.type;
			bool fp = ty.IsFloat();

			switch (ast.prefix.kind)
			{
			case TokenKind.Dash:
				op = fp ? UnOp.FNeg : UnOp.Neg;
				goto UnExpr;
			case TokenKind.At:
				op = UnOp.Ref;
				ty.Ptr();
				goto UnExpr;
			case TokenKind.Percent:
				op = UnOp.Deref;
				ty = ty.ElementType;
				goto UnExpr;

			default:
				throw new InvalidOperationException("Invalid un-expr");
			}

		UnExpr:
			return new UnExprNode(ty, op, var) { syntax = ast };
		}

		private ExprNode Nodify(TypePunExprAst ast)
		{
			var ty = Nodify(ast.type);
			var expr = Nodify(ast.expr);
			if (ty == expr.type)
				return expr;
			return new TypePunExprNode(ty, expr) { syntax = ast };
		}

		private ExprNode Cast(ExprNode node, LLVMTypeRef to)
		{
			if (to == default || node.type == to)
				return node;

			if (to.IntWidth < node.type.IntWidth && !to.IsFloat() && !to.IsPtr())
				safety.ReportPossibleLossOfData(node.syntax.token);
			return new CastExprNode(node, to);
		}

		private FieldNode Mem(LLVMTypeRef ty, Token name)
		{
			if (ty == null)
				return null;
			var ztruct = ztructs[ty];
			foreach (var field in ztruct.fields)
				if (field.name == name.text)
					return field;
			safety.ReportNSField(name);
			return null;
		}

		private bool IsMut(Token? mutKey)
		{
			if (mutKey is null)
				return !syc.defConst;
			return mutKey.Value.kind is TokenKind.MutKey;

			throw new InvalidOperationException("Can't deduce whether mutable");
		}

		private Visibility Vis(Token? token)
		{
			if (token is null)
				return syc.defVis;
			switch (token?.kind)
			{
			case TokenKind.PubKey:
				return Visibility.Public;
			case TokenKind.ProtKey:
				return Visibility.Protected;
			case TokenKind.PrivKey:
				return Visibility.Private;

			default:
				return syc.defVis;
			}
		}
	}
}
