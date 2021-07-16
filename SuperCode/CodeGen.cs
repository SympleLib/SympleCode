using System;
using System.Collections.Generic;

using LLVMSharp.Interop;

namespace SuperCode
{
	public class CodeGen
	{
		private static readonly Dictionary<string, LLVMValueRef> vars = new ();

		public readonly ModuleNode modNode;
		public readonly LLVMModuleRef module;
		private readonly LLVMBuilderRef builder;

		public CodeGen(ModuleNode mod)
		{
			modNode = mod;
			module = LLVMModuleRef.CreateWithName(mod.filename);
			builder = LLVMBuilderRef.Create(module.Context);
		}

		public LLVMModuleRef Gen()
		{
			foreach (var mem in modNode.mems)
				Gen(mem);
			return module;
		}

		private LLVMValueRef Gen(Node node)
		{
			switch (node.kind)
			{
			case NodeKind.FuncMem:
				return Gen((FuncMemNode) node);
			case NodeKind.VarStmt:
				return Gen((VarStmtNode) node);
			case NodeKind.NumExpr:
				return Gen((NumExprNode) node);
			case NodeKind.BinExpr:
				return Gen((BinExprNode) node);
			case NodeKind.VarExpr:
				return Gen((VarExprNode) node);

			default:
				throw new InvalidOperationException("Invalid node");
			}
		}

		private LLVMValueRef Gen(FuncMemNode mem)
		{
			vars.Clear();

			var ty = LLVMTypeRef.CreateFunction(LLVMTypeRef.Void, Array.Empty<LLVMTypeRef>());
			var fn = module.AddFunction(mem.name, ty);
			var entry = fn.AppendBasicBlock("Entry");
			builder.PositionAtEnd(entry);
			foreach (var stmt in mem.stmts)
				Gen(stmt);

			builder.BuildRetVoid();
			return fn;
		}

		private LLVMValueRef Gen(VarStmtNode stmt)
		{
			var ptr = builder.BuildAlloca(LLVMTypeRef.Int32);
			var init = Gen(stmt.init);
			builder.BuildStore(init, ptr);
			vars.Add(stmt.name, ptr);
			return ptr;
		}

		private LLVMValueRef Gen(NumExprNode expr) =>
			LLVMValueRef.CreateConstInt(LLVMTypeRef.Int32, expr.value);

		private LLVMValueRef Gen(BinExprNode expr)
		{
			var left = Gen(expr.left);
			var right = Gen(expr.right);

			switch (expr.op)
			{
			case BinOp.Add:
				return builder.BuildAdd(left, right);
			case BinOp.Sub:
				return builder.BuildSub(left, right);
			case BinOp.Mul:
				return builder.BuildMul(left, right);
			case BinOp.Div:
				return builder.BuildSDiv(left, right);
			case BinOp.Mod:
				return builder.BuildSRem(left, right);

			default:
				throw new InvalidOperationException("Invalid bin-expr");
			}
		}

		private LLVMValueRef Gen(VarExprNode expr) =>
			builder.BuildLoad(vars[expr.name]);
	}
}
