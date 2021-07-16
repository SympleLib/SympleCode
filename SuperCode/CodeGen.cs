using System;
using System.Collections.Generic;

using LLVMSharp.Interop;

namespace SuperCode
{
	public class CodeGen
	{
		public readonly ModuleNode modNode;
		public readonly LLVMModuleRef module;
		private readonly LLVMBuilderRef builder;

		public CodeGen(ModuleNode mod)
		{
			modNode = mod;
			module = LLVMModuleRef.CreateWithName(mod.filename);
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

			default:
				throw new InvalidOperationException("Invalid node");
			}
		}

		private LLVMValueRef Gen(FuncMemNode mem)
		{
			var ty = LLVMTypeRef.CreateFunction(LLVMTypeRef.Int32, Array.Empty<LLVMTypeRef>());
			var fn = module.AddFunction(mem.name, ty);
			var entry = fn.AppendBasicBlock("Entry");
			builder.PositionAtEnd(entry);
			foreach (var stmt in mem.stmts)
				Gen(stmt);
			return fn;
		}

		private LLVMValueRef Gen(VarStmtNode stmt)
		{
			var ptr = builder.BuildAlloca(LLVMTypeRef.Int32);
			var init = Gen(stmt.init);
			builder.BuildStore(init, ptr);
			return ptr;
		}
	}
}
