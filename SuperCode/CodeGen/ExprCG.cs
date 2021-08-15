using LLVMSharp.Interop;

using static SuperCode.BuiltinTypes;
using static LLVMSharp.Interop.LLVMIntPredicate;
using static LLVMSharp.Interop.LLVMRealPredicate;

namespace SuperCode
{
	public partial class CodeGen
	{
		private LLVMValueRef Gen(ArrExprNode node)
		{
			var elements = new LLVMValueRef[node.elements.Length];
			var arr = builder.BuildAlloca(LLVMTypeRef.CreateArray(node.type.ElementType, (uint) elements.Length), "..arr");
			var ptr = builder.BuildBitCast(arr, node.type);

			for (int i = 0; i < elements.Length; i++)
			{
				var index = builder.BuildInBoundsGEP(ptr, new LLVMValueRef[] { LLVMValueRef.CreateConstInt(LLVMTypeRef.Int64, (ulong) i) });
				elements[i] = Gen(node.elements[i]);
				builder.BuildStore(elements[i], index);
			}

			return ptr;
		}

		private LLVMValueRef Gen(AssignExprNode node)
		{
			var expr = GenAddr(node.expr);
			var val = Gen(node.value);
			builder.BuildStore(val, expr);
			return builder.BuildLoad(expr);
		}

		private LLVMValueRef Gen(BinExprNode node)
		{
			var left = Gen(node.left);
			var right = Gen(node.right);

			switch (node.op)
			{
			case BinOp.Add:
				return builder.BuildAdd(left, right);
			case BinOp.Sub:
				return builder.BuildSub(left, right);
			case BinOp.Mul:
				return builder.BuildMul(left, right);
			case BinOp.SDiv:
				return builder.BuildSDiv(left, right);
			case BinOp.SMod:
				return builder.BuildSRem(left, right);
			case BinOp.UDiv:
				return builder.BuildUDiv(left, right);
			case BinOp.UMod:
				return builder.BuildURem(left, right);

			case BinOp.FAdd:
				return builder.BuildFAdd(left, right);
			case BinOp.FSub:
				return builder.BuildFSub(left, right);
			case BinOp.FMul:
				return builder.BuildFMul(left, right);
			case BinOp.FDiv:
				return builder.BuildFDiv(left, right);
			case BinOp.FMod:
				return builder.BuildFRem(left, right);

			case BinOp.Eql:
				return builder.BuildICmp(LLVMIntEQ, left, right);
			case BinOp.NEql:
				return builder.BuildICmp(LLVMIntNE, left, right);
			case BinOp.SLt:
				return builder.BuildICmp(LLVMIntSLT, left, right);
			case BinOp.SGt:
				return builder.BuildICmp(LLVMIntSGT, left, right);
			case BinOp.SLtEql:
				return builder.BuildICmp(LLVMIntSLE, left, right);
			case BinOp.SGtEql:
				return builder.BuildICmp(LLVMIntSGE, left, right);

			case BinOp.ULt:
				return builder.BuildICmp(LLVMIntULT, left, right);
			case BinOp.UGt:
				return builder.BuildICmp(LLVMIntUGT, left, right);
			case BinOp.ULtEql:
				return builder.BuildICmp(LLVMIntULE, left, right);
			case BinOp.UGtEql:
				return builder.BuildICmp(LLVMIntUGE, left, right);

			case BinOp.FEql:
				return builder.BuildFCmp(LLVMRealOEQ, left, right);
			case BinOp.FNEql:
				return builder.BuildFCmp(LLVMRealONE, left, right);
			case BinOp.FLt:
				return builder.BuildFCmp(LLVMRealOLT, left, right);
			case BinOp.FGt:
				return builder.BuildFCmp(LLVMRealOGT, left, right);
			case BinOp.FLtEql:
				return builder.BuildFCmp(LLVMRealOLE, left, right);
			case BinOp.FGtEql:
				return builder.BuildFCmp(LLVMRealOGE, left, right);

			case BinOp.Index:
				return builder.BuildLoad(builder.BuildInBoundsGEP(left, new LLVMValueRef[] { right }));
			case BinOp.And:
				return builder.BuildAnd(left, right);
			case BinOp.Or:
				return builder.BuildOr(left, right);

			default:
				throw new InvalidOperationException("Invalid bin-expr");
			}
		}

		private LLVMValueRef Gen(CallExprNode expr)
		{
			var what = Gen(expr.what);
			var args = new LLVMValueRef[expr.args.Length];
			for (int i = 0; i < args.Length; i++)
				args[i] = Gen(expr.args[i]);

			var call = builder.BuildCall(what, args);
			if (what.TypeOf.ReturnType == builtinTypes["^"])
			{
				builder.BuildUnreachable();
				//returned = true;
			}
			return call;
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

			if (from.IsRef())
				return builder.BuildLoad(val);

			if (from.IsPtr() && to.IsPtr())
				return builder.BuildPointerCast(val, to);
			if (from.IsPtr() && !to.IsPtr())
				return builder.BuildPtrToInt(val, to);
			if (!from.IsPtr() && to.IsPtr())
				return builder.BuildIntToPtr(val, to);
			return builder.BuildIntCast(val, to); ;
		}

		private LLVMValueRef Gen(FNumExprNode node) =>
			LLVMValueRef.CreateConstReal(node.type, node.value);

		private LLVMValueRef Gen(FieldExprNode node)
		{
			var ptr = GenAddr(node.expr);
			var field = builder.BuildStructGEP(ptr, (uint) node.field.index);
			return builder.BuildLoad(field);
		}

		private LLVMValueRef Gen(NumExprNode node) =>
			LLVMValueRef.CreateConstInt(node.type, node.value);

		private LLVMValueRef Gen(StrExprNode node)
		{
			var values = new LLVMValueRef[node.str.Length + 1];
			for (int i = 0; i < node.str.Length; i++)
				values[i] = LLVMValueRef.CreateConstInt(node.type.ElementType, node.str[i]);
			values[^1] = LLVMValueRef.CreateConstInt(node.type.ElementType, 0);
			var arr = LLVMValueRef.CreateConstArray(node.type.ElementType, values);

			var str = module.AddGlobal(arr.TypeOf, "..str");
			str.Linkage = LLVMLinkage.LLVMPrivateLinkage;
			str.IsGlobalConstant = true;
			str.HasUnnamedAddr = true;
			str.Initializer = arr;
			return builder.BuildBitCast(str, node.type);
		}

		private LLVMValueRef Gen(SymExprNode node)
		{
			LLVMValueRef sym;
			if (node.symbol is DeclFuncMemNode decl && decl.impl is not null)
				sym = syms[decl.impl];
			else
				sym = syms[node.symbol];

			var symbol = (Node) node.symbol;
			if (symbol.kind is NodeKind.VarStmt or NodeKind.VarMem or NodeKind.Field)
				return builder.BuildLoad(sym);
			return sym;
		}

		private LLVMValueRef Gen(TypePunExprNode node)
		{
			var expr = Gen(node.expr);
			return builder.BuildBitCast(expr, node.type);
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
			case UnOp.FNeg:
				return builder.BuildFNeg(expr);
			case UnOp.Deref:
				return builder.BuildLoad(expr);
			case UnOp.Not:
				return builder.BuildNot(expr);

			default:
				throw new InvalidOperationException("Invalid un-expr");
			}
		}
	}
}
