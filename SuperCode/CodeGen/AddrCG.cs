using LLVMSharp.Interop;
using System;

namespace SuperCode
{
	public partial class CodeGen
	{
		private LLVMValueRef GenAddr(Node? node)
		{
			if (node is null)
				return LLVMValueRef.CreateConstNull(LLVMTypeRef.Void);

			switch (node.kind)
			{
			case NodeKind.MemExpr:
				return GenAddr((FieldExprNode) node);
			case NodeKind.SymExpr:
				return GenAddr((SymExprNode) node);
			case NodeKind.BinExpr:
				return GenAddr((BinExprNode) node);
			case NodeKind.UnExpr:
				return GenAddr((UnExprNode) node);

			default:
				throw new InvalidOperationException("Not an addr");
			}
		}

		private LLVMValueRef GenAddr(FieldExprNode node)
		{
			var ptr = GenAddr(node.expr);
			return builder.BuildStructGEP(ptr, (uint) node.field.index);
		}

		private LLVMValueRef GenAddr(SymExprNode node)
		{
			LLVMValueRef sym;
			if (node.symbol is DeclFuncMemNode decl && decl.impl is not null)
				sym = syms[decl.impl];
			else
				sym = syms[node.symbol];

			if (sym.TypeOf.ElementType.IsRef())
				return builder.BuildLoad(sym);
			return sym;
		}

		private LLVMValueRef GenAddr(UnExprNode node)
		{
			if (node.op is not UnOp.Deref)
				throw new InvalidOperationException("Not an addr");
			return Gen(node.expr);
		}

		private LLVMValueRef GenAddr(BinExprNode node)
		{
			if (node.op is not BinOp.Index)
				throw new InvalidOperationException("Not an addr");
			var left = Gen(node.left);
			var right = Gen(node.right);
			return builder.BuildInBoundsGEP(left, new LLVMValueRef[] { right });
		}
	}
}
