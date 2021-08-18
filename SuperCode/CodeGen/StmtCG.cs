using LLVMSharp.Interop;

namespace SuperCode
{
	public partial class CodeGen
	{
		private LLVMValueRef Gen(AsmStmtNode node)
		{
			var fnTy = LLVMTypeRef.CreateFunction(LLVMTypeRef.Void, Array.Empty<LLVMTypeRef>());
			var fn = LLVMValueRef.CreateConstInlineAsm(fnTy, node.asm, "", false, false);
			return builder.BuildCall(fn, Array.Empty<LLVMValueRef>());
		}

		private LLVMValueRef Gen(BlockStmtNode node)
		{
			foreach (var stmt in node.stmts)
				Gen(stmt);
			return null;
		}

		private LLVMValueRef Gen(IfStmtNode node)
		{
			bool hasElse = node.elze is not null;

			var cond = Gen(node.cond);
			var then = func.AppendBasicBlock();
			var elze = hasElse ? func.AppendBasicBlock() : default;
			var end = func.AppendBasicBlock();
			var branch = builder.BuildCondBr(cond, then, hasElse ? elze : end);

			builder.PositionAtEnd(then);
			returned = false;
			Gen(node.then);
			if (!returned)
				builder.BuildBr(end);

			if (node.elze is not null)
			{
				builder.PositionAtEnd(elze);
				returned = false;
				Gen(node.elze);
				if (!returned)
					builder.BuildBr(end);
			}

			builder.PositionAtEnd(end);
			returned = false;
			return branch;
		}

		private LLVMValueRef Gen(RetStmtNode node)
		{
			var val = Gen(node.value);
			returned = true;
			if (node.value is null)
				return builder.BuildRetVoid();
			return builder.BuildRet(val);
		}

		private LLVMValueRef Gen(TypedefStmtNode node) => null;

		private LLVMValueRef Gen(VarStmtNode node)
		{
			var ptr = builder.BuildAlloca(node.type, node.name);
			LLVMValueRef init;

			if (node.init is null)
				init = LLVMValueRef.CreateConstNull(node.type);
			else if (node.type.IsRef())
				init = GenAddr(node.init);
			else
				init = Gen(node.init);

			builder.BuildStore(init, ptr);
			syms.Add(node, ptr);
			return ptr;
		}

		private LLVMValueRef Gen(WhileStmtNode node)
		{
			var loop = func.AppendBasicBlock();
			var then = func.AppendBasicBlock();
			var end = func.AppendBasicBlock();
			builder.BuildBr(loop);

			builder.PositionAtEnd(loop);
			var cond = Gen(node.cond);
			var branch = builder.BuildCondBr(cond, then, end);
			builder.PositionAtEnd(then);
			Gen(node.then);
			builder.BuildBr(loop);

			builder.PositionAtEnd(end);
			return branch;
		}
	}
}
