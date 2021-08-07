using LLVMSharp.Interop;
using System.Collections.Generic;
using System;

using static SuperCode.BuiltinTypes;

namespace SuperCode
{
	public partial class Noder
	{
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
			case AstKind.UsingStmt:
				return Nodify((UsingStmtAst) ast);
			case AstKind.VarStmt:
				return Nodify((VarStmtAst) ast);
			case AstKind.WhileStmt:
				return Nodify((WhileStmtAst) ast);


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
			var cond = Nodify(ast.cond, builtinTypes["bool"]);
			var stmts = new Node[ast.then.Length];
			for (int i = 0; i < stmts.Length; i++)
				stmts[i] = Nodify(ast.then[i]);
			var then = new BlockStmtNode(stmts);
			Node? elze = null;
			if (ast.elze is not null)
				elze = Nodify(ast.elze);

			return new IfStmtNode(cond, then, elze);
		}

		private RetStmtNode Nodify(RetStmtAst ast)
		{
			if (retType == LLVMTypeRef.Void)
			{
				if (ast.value is not null)
					throw new Exception("Must return nothing");
				return new RetStmtNode(null);
			}

			if (ast.value is null)
				throw new Exception("Must return something");
			return new RetStmtNode(Nodify(ast.value!, retType));
		}

		private TypedefStmtNode Nodify(UsingStmtAst ast)
		{
			string alias = ast.alias.text;
			var real = Type(ast.real);
			types.Add(alias, real);
			return new TypedefStmtNode(alias, real);
		}

		private VarStmtNode Nodify(VarStmtAst ast)
		{
			var ty = Type(ast.type);
			var initTy = ty.IsRef() ? default : ty;
			var init = ast.init is null ? null : Nodify(ast.init, initTy);
			var var = new VarStmtNode(ty, IsMut(ast.mutKey), ast.name.text, init) { syntax = ast };
			syms.Add(var.name, var);
			return var;
		}

		private WhileStmtNode Nodify(WhileStmtAst ast)
		{
			var cond = Nodify(ast.cond, builtinTypes["bool"]);
			var stmts = new Node[ast.then.Length];
			for (int i = 0; i < stmts.Length; i++)
				stmts[i] = Nodify(ast.then[i]);
			var then = new BlockStmtNode(stmts);

			return new WhileStmtNode(cond, then);
		}
	}
}
