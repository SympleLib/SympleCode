﻿using System;
using System.Collections.Generic;

using LLVMSharp.Interop;

namespace SuperCode
{
	public class CodeGen
	{
		private readonly Dictionary<Symbol, LLVMValueRef> syms = new ();

		public readonly ModuleNode modNode;
		public readonly LLVMModuleRef module;
		private readonly LLVMBuilderRef builder;
		private readonly LLVMDIBuilderRef dbuilder;
		private LLVMMetadataRef file;

		public CodeGen(ModuleNode mod)
		{
			modNode = mod;
			module = LLVMModuleRef.CreateWithName(mod.filename);
			builder = LLVMBuilderRef.Create(module.Context);
			dbuilder = module.CreateDIBuilder();
		}

		public LLVMModuleRef Gen()
		{
			file = dbuilder.CreateFile(modNode.filename, "");
			dbuilder.CreateCompileUnit(LLVMDWARFSourceLanguage.LLVMDWARFSourceLanguageC, file,
				"SuperCode d1.0.0", 0, "", 0, "", LLVMDWARFEmissionKind.LLVMDWARFEmissionFull, 0, 0, 1, "", "");
			foreach (var mem in modNode.mems)
				Gen(mem);
			dbuilder.DIBuilderFinalize();
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
			case NodeKind.DeclFuncMem:
				return Gen((DeclFuncMemNode) node);

			case NodeKind.RetStmt:
				return Gen((RetStmtNode) node);
			case NodeKind.VarStmt:
				return Gen((VarStmtNode) node);

			case NodeKind.BinExpr:
				return Gen((BinExprNode) node);
			case NodeKind.CallExpr:
				return Gen((CallExprNode) node);
			case NodeKind.CastExpr:
				return Gen((CastExprNode) node);
			case NodeKind.FNumExpr:
				return Gen((FNumExprNode) node);
			case NodeKind.NumExpr:
				return Gen((NumExprNode) node);
			case NodeKind.UnExpr:
				return Gen((UnExprNode) node);
			case NodeKind.StrExpr:
				return Gen((StrExprNode) node);
			case NodeKind.SymExpr:
				return Gen((SymExprNode) node);

			default:
				throw new InvalidOperationException("Invalid node");
			}
		}

		private LLVMValueRef Gen(FuncMemNode mem)
		{
			var fn = module.AddFunction(mem.name, mem.type);
			var entry = fn.AppendBasicBlock();
			builder.PositionAtEnd(entry);
			syms.Add(mem, fn);

			for (int i = 0; i < mem.paramz.Length; i++)
				syms.Add(mem.paramz[i], fn.Params[i]);

			foreach (var stmt in mem.stmts)
				Gen(stmt);

			if (mem.type.ReturnType == LLVMTypeRef.Void)
				builder.BuildRetVoid();
			return fn;
		}

		private LLVMValueRef Gen(DeclFuncMemNode mem)
		{
			var fn = module.AddFunction(mem.name, mem.type);
			syms.Add(mem, fn);
			return fn;
		}

		private LLVMValueRef Gen(RetStmtNode stmt) =>
			builder.BuildRet(Gen(stmt.value));

		private LLVMValueRef Gen(VarStmtNode stmt)
		{
			var ptr = builder.BuildAlloca(stmt.type);
			var init = Gen(stmt.init);
			builder.BuildStore(init, ptr);
			syms.Add(stmt, ptr);
			return ptr;
		}

		private LLVMValueRef Gen(NumExprNode expr) =>
			LLVMValueRef.CreateConstInt(expr.type, expr.value);

		private LLVMValueRef Gen(FNumExprNode expr) =>
			LLVMValueRef.CreateConstReal(expr.type, expr.value);

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

		private LLVMValueRef Gen(StrExprNode node) =>
			builder.BuildGlobalStringPtr(node.str);

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
			for (int i = 0; i < what.Params.Length; i++)
				args.Add(Gen(expr.args[i]));

			return builder.BuildCall(what, args.ToArray());
		}

		private LLVMValueRef Gen(CastExprNode expr)
		{
			var val = Gen(expr.value);
			var from = val.TypeOf;
			var to = expr.type;
			if (from.IsFloat() && !to.IsFloat())
				return builder.BuildFPToSI(val, to);
			if (!from.IsFloat() && to.IsFloat())
				return builder.BuildSIToFP(val, to);
			if (from.IsFloat() && to.IsFloat())
				return builder.BuildFPCast(val, to);

			if (from.IsPtr() && to.IsPtr())
				return builder.BuildPointerCast(val, to);
			if (from.IsPtr() && !to.IsPtr())
				return builder.BuildPtrToInt(val, to);
			if (!from.IsPtr() && to.IsPtr())
				return builder.BuildIntToPtr(val, to);
			return builder.BuildIntCast(val, to); ;
		}

		private LLVMValueRef Gen(UnExprNode node)
		{
			if (node.op is UnOp.Ref)
				return GenAddr(node.expr);

			var expr = Gen(node.expr);

			switch (node.op)
			{
			case UnOp.Neg:
				return builder.BuildNeg(expr);
			case UnOp.Deref:
				return builder.BuildLoad(expr);

			default:
				throw new InvalidOperationException("Invalid un-expr");
			}
		}

		private LLVMValueRef GenAddr(Node node)
		{
			if (node.kind != NodeKind.SymExpr)
				throw new InvalidOperationException("Not an addr");
			return syms[((SymExprNode) node).symbol];
		}
	}
}
