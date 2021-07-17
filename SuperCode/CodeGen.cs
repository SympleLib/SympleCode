using System;
using System.Collections.Generic;

using LLVMSharp.Interop;

namespace SuperCode
{
	public class CodeGen
	{
		private static readonly Dictionary<VarStmtNode, LLVMValueRef> vars = new ();
		private static readonly Dictionary<FuncMemNode, LLVMValueRef> funcs = new ();

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

		public void Optimize()
		{
			var pass = LLVMPassManagerRef.Create();
			pass.AddGVNPass();
			pass.AddReassociatePass();
			pass.AddCFGSimplificationPass();
			pass.AddInstructionCombiningPass();
			pass.InitializeFunctionPassManager();

			foreach (var func in funcs)
				pass.RunFunctionPassManager(func.Value);
			pass.Run(module);
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
			var ptr = builder.BuildAlloca(stmt.type);
			var init = Gen(stmt.init);
			builder.BuildStore(GenCast(init, stmt.type), ptr);
			vars.Add(stmt, ptr);
			return ptr;
		}

		private LLVMValueRef Gen(NumExprNode expr) =>
			LLVMValueRef.CreateConstInt(expr.type, expr.value);

		private LLVMValueRef Gen(BinExprNode expr)
		{
			var left = GenCast(Gen(expr.left), expr.type);
			var right = GenCast(Gen(expr.right), expr.type);

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
			builder.BuildLoad(vars[expr.symbol]);

		private LLVMValueRef GenCast(LLVMValueRef val, LLVMTypeRef to)
		{
			var from = val.TypeOf;
			if (from.IsFloat() && !to.IsFloat())
				return builder.BuildFPToSI(val, to);
			if (!from.IsFloat() && to.IsFloat())
				return builder.BuildSIToFP(val, to);
			if (from.IsFloat() && to.IsFloat())
				return builder.BuildFPCast(val, to);

			return builder.BuildIntCast(val, to); ;
		}
	}
}
