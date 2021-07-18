using System;
using System.Collections.Generic;

using LLVMSharp.Interop;

namespace SuperCode
{
	public class CodeGen
	{
		private readonly Dictionary<Symbol, LLVMValueRef> syms = new ();
		private FuncMemNode func;

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
			pass.Run(module);
		}

		private LLVMValueRef Gen(Node node)
		{
			switch (node.kind)
			{
			case NodeKind.FuncMem:
				return Gen((FuncMemNode) node);

			case NodeKind.RetStmt:
				return Gen((RetStmtNode) node);
			case NodeKind.VarStmt:
				return Gen((VarStmtNode) node);

			case NodeKind.NumExpr:
				return Gen((NumExprNode) node);
			case NodeKind.FNumExpr:
				return Gen((FNumExprNode) node);
			case NodeKind.BinExpr:
				return Gen((BinExprNode) node);
			case NodeKind.SymExpr:
				return Gen((SymExprNode) node);
			case NodeKind.CallExpr:
				return Gen((CallExprNode) node);

			default:
				throw new InvalidOperationException("Invalid node");
			}
		}

		private LLVMValueRef Gen(FuncMemNode mem)
		{
			func = mem;
			var fn = module.AddFunction(mem.name, mem.type);
			var entry = fn.AppendBasicBlock("Entry");
			builder.PositionAtEnd(entry);
			syms.Add(func, fn);

			for (int i = 0; i < mem.paramz.Length; i++)
				syms.Add(mem.paramz[i], fn.Params[i]);

			foreach (var stmt in mem.stmts)
				Gen(stmt);

			if (mem.type.ReturnType == LLVMTypeRef.Void)
				builder.BuildRetVoid();
			return fn;
		}

		private LLVMValueRef Gen(RetStmtNode stmt) =>
			builder.BuildRet(GenCast(Gen(stmt.value), func.type.ReturnType));

		private LLVMValueRef Gen(VarStmtNode stmt)
		{
			var ptr = builder.BuildAlloca(stmt.type);
			var init = Gen(stmt.init);
			builder.BuildStore(GenCast(init, stmt.type), ptr);
			syms.Add(stmt, ptr);
			return ptr;
		}

		private LLVMValueRef Gen(NumExprNode expr) =>
			LLVMValueRef.CreateConstInt(expr.type, expr.value);

		private LLVMValueRef Gen(FNumExprNode expr) =>
			LLVMValueRef.CreateConstReal(expr.type, expr.value);

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

		private LLVMValueRef Gen(SymExprNode expr)
		{
			var sym = syms[expr.symbol];
			if (((Node) expr.symbol).kind == NodeKind.VarStmt)
				return builder.BuildLoad(sym);
			return sym;
		}

		private LLVMValueRef Gen(CallExprNode expr)
		{
			var what = Gen(expr.what);
			var args = new List<LLVMValueRef>();
			foreach (var arg in expr.args)
				args.Add(Gen(arg));

			return builder.BuildCall(what, args.ToArray());
		}

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
