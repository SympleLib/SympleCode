using LLVMSharp.Interop;
using System;

using static SuperCode.BuiltinTypes;

namespace SuperCode
{
	public partial class Noder
	{
		private ExprNode Nodify(ExprAst ast, LLVMTypeRef castTo = default)
		{
			if (ast is null)
				return Cast(new NumExprNode(0, builtinTypes["bit"]), castTo);

			switch (ast.kind)
			{
			case AstKind.ArrExpr:
				return Cast(Nodify((ArrExprAst) ast), castTo);
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

		private ArrExprNode Nodify(ArrExprAst ast)
		{
			var type = builtinTypes["byte"].Ptr();
			var elements = new ExprNode[ast.elements.Length];
			if (elements.Length > 0)
			{
				var element = Nodify(ast.elements[0]);
				elements[0] = element;
				type = element.type.Ptr();
			}

			for (int i = 1; i < elements.Length; i++)
				elements[i] = Nodify(ast.elements[i]);
			return new ArrExprNode(type, elements);
		}

		private ExprNode Nodify(BinExprAst ast)
		{
			var left = Nodify(ast.left);
			switch (ast.op.kind)
			{
			case TokenKind.Dot:
			{
				var name = ast.right.token;
				var field = Field(left.type, name);
				return new FieldExprNode(left, field is null ? FieldNode.nullField : field);
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

			case TokenKind.And:
				op = BinOp.And;
				goto BinExpr;
			case TokenKind.Pipe:
				op = BinOp.Or;
				goto BinExpr;

			default:
				throw new InvalidOperationException("Invalid bin-expr");
			}

		LogExpr:
			return new BinExprNode(op, left, right, builtinTypes["bool"]);
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
			var to = Type(ast.type);
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
			case TokenKind.TrueKey:
				return new NumExprNode(1, builtinTypes["bool"]) { syntax = ast };
			case TokenKind.FalseKey:
				return new NumExprNode(0, builtinTypes["bool"]) { syntax = ast };
			case TokenKind.NullKey:
				// Chez
				return new NumExprNode(0, builtinTypes["bit"]) { syntax = ast };
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
					's' => builtinTypes["schar"].Ptr(),
					'u' => builtinTypes["char"].Ptr(),
					'w' => builtinTypes["wchar"].Ptr(),

					_ => builtinTypes["char"].Ptr(),
				};

				return new StrExprNode(type, literal[start..^1]) { syntax = ast };
			}
			case TokenKind.Char:
			{
				var type = literal[0] switch
				{
					's' => builtinTypes["schar"],
					'u' => builtinTypes["char"],
					'w' => builtinTypes["wchar"],

					_ => builtinTypes["char"],
				};
				return new NumExprNode(literal[literal[0] == '`' ? 1 : 2], type);
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
			if (ast.prefix.kind is TokenKind.Plus)
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
			case TokenKind.Bang:
				op = UnOp.Not;
				goto LogExpr;

			default:
				throw new InvalidOperationException("Invalid un-expr");
			}

		LogExpr:
			return new UnExprNode(builtinTypes["bool"], op, Cast(var, builtinTypes["bool"])) { syntax = ast };
		UnExpr:
			return new UnExprNode(ty, op, var) { syntax = ast };
		}

		private ExprNode Nodify(TypePunExprAst ast)
		{
			var ty = Type(ast.type);
			var expr = Nodify(ast.expr);
			if (ty == expr.type)
				return expr;
			return new TypePunExprNode(ty, expr) { syntax = ast };
		}
	}
}
