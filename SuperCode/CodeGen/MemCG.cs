﻿using LLVMSharp.Interop;

namespace SuperCode
{
	public partial class CodeGen
	{
		private LLVMValueRef Gen(DeclFuncMemNode node)
		{
			if (module.GetNamedFunction(node.name) != null)
				return node.impl is null ? syms[node] : syms[node.impl];

			var fn = module.AddFunction(node.name, node.type);
			if (node.impl is null)
				syms.Add(node, fn);
			else
				syms.Add(node.impl, fn);
			return fn;
		}

		private LLVMTypeRef Gen(DeclStructMemNode node)
		{
			if (node.impl is null)
				throw new InvalidOperationException("Struct no impl");
			return module.GetTypeByName(node.name);
		}

		private LLVMValueRef Gen(DeclVarMemNode node)
		{
			if (module.GetNamedGlobal(node.name) != null)
				return node.impl is null ? syms[node] : syms[node.impl];

			var var = module.AddGlobal(node.type, node.name);
			if (node.impl is null)
				syms.Add(node, var);
			else
				syms.Add(node.impl, var);
			return var;
		}

		private LLVMValueRef Gen(ImportMemNode node)
		{
			foreach (var mem in node.module.mems)
				Gen(mem, true);
			return null;
		}

		private LLVMValueRef Gen(FuncMemNode node, bool extrn = false)
		{
			func = syms[node];
			if (extrn)
				return func;

			node.vis.Apply(func);
			var entry = func.AppendBasicBlock();
			builder.PositionAtEnd(entry);

			for (int i = 0; i < node.paramz.Length; i++)
			{
				var param = func.Params[i];
				var ptr = builder.BuildAlloca(param.TypeOf);
				builder.BuildStore(param, ptr);
				ptr.Name = node.paramz[i].name;

				syms.Add(node.paramz[i], ptr);
			}

			foreach (var stmt in node.stmts)
				Gen(stmt);

			if (node.retType == LLVMTypeRef.Void && !returned)
				builder.BuildRetVoid();
			else if (node.noret && !returned)
				builder.BuildUnreachable();
			returned = false;
			var fn = func;
			func = null;
			return fn;
		}

		private LLVMValueRef Gen(StructMemNode node) =>
			null;

		private LLVMValueRef Gen(VarMemNode node, bool extrn = false)
		{
			var var = module.AddGlobal(node.type, node.name);
			syms.Add(node, var);
			if (extrn)
				return var;

			var.IsGlobalConstant = !node.mut;
			node.vis.Apply(var);
			if (node.init is null)
				var.Initializer = LLVMValueRef.CreateConstNull(node.type);
			else
				var.Initializer = Gen(node.init);
			return var;
		}
	}
}
