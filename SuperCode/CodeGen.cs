using System;
using System.Collections.Generic;

using LLVMSharp.Interop;
using static LLVMSharp.Interop.LLVMIntPredicate;
using static LLVMSharp.Interop.LLVMRealPredicate;

namespace SuperCode
{
	public class CodeGen
	{
		private LLVMValueRef func;
		private readonly Dictionary<Symbol, LLVMValueRef> syms = new Dictionary<Symbol, LLVMValueRef>();

		public readonly ModuleNode modNode;
		public readonly LLVMModuleRef module;
		private readonly LLVMBuilderRef builder;
		private readonly LLVMDIBuilderRef dbuilder;
		private LLVMMetadataRef file;
		private bool returned;

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
			pass.AddAggressiveDCEPass();
			pass.AddAlignmentFromAssumptionsPass();
			pass.AddAlwaysInlinerPass();
			pass.AddArgumentPromotionPass();
			pass.AddBasicAliasAnalysisPass();
			pass.AddBitTrackingDCEPass();
			pass.AddCalledValuePropagationPass();
			pass.AddCFGSimplificationPass();
			pass.AddConstantMergePass();
			pass.AddCorrelatedValuePropagationPass();
			pass.AddDCEPass();
			pass.AddDeadArgEliminationPass();
			pass.AddDeadStoreEliminationPass();
			pass.AddDemoteMemoryToRegisterPass();
			pass.AddEarlyCSEMemSSAPass();
			pass.AddEarlyCSEPass();
			pass.AddFunctionAttrsPass();
			pass.AddFunctionInliningPass();
			pass.AddGlobalDCEPass();
			pass.AddGlobalOptimizerPass();
			pass.AddGVNPass();
			pass.AddIndVarSimplifyPass();
			pass.AddInstructionCombiningPass();
			pass.AddIPSCCPPass();
			pass.AddJumpThreadingPass();
			pass.AddLICMPass();
			pass.AddLoopDeletionPass();
			pass.AddLoopIdiomPass();
			pass.AddLoopRerollPass();
			pass.AddLoopRotatePass();
			pass.AddLoopUnrollPass();
			pass.AddLoopUnswitchPass();
			pass.AddLoopVectorizePass();
			pass.AddLowerConstantIntrinsicsPass();
			pass.AddLowerExpectIntrinsicPass();
			pass.AddLowerSwitchPass();
			pass.AddMemCpyOptPass();
			pass.AddMergedLoadStoreMotionPass();
			pass.AddMergeFunctionsPass();
			pass.AddNewGVNPass();
			pass.AddPartiallyInlineLibCallsPass();
			pass.AddPromoteMemoryToRegisterPass();
			pass.AddPruneEHPass();
			pass.AddReassociatePass();
			pass.AddScalarizerPass();
			pass.AddScalarReplAggregatesPass();
			pass.AddScalarReplAggregatesPassSSA();
			pass.AddSCCPPass();
			pass.AddScopedNoAliasAAPass();
			pass.AddSimplifyLibCallsPass();
			pass.AddSLPVectorizePass();
			pass.AddStripDeadPrototypesPass();
			pass.AddStripSymbolsPass();
			pass.AddTailCallEliminationPass();
			pass.AddTypeBasedAliasAnalysisPass();
			pass.AddVerifierPass();
			pass.InitializeFunctionPassManager();
			pass.Run(module);
		}

		private LLVMValueRef Gen(Node? node, bool extrn = false)
		{
			if (node is null || returned)
				return null;


			switch (node.kind)
			{
			case NodeKind.DeclFuncMem:
				return Gen((DeclFuncMemNode) node);
			case NodeKind.ImportMem:
				return Gen((ImportMemNode) node);
			case NodeKind.FuncMem:
				return Gen((FuncMemNode) node, extrn);
			case NodeKind.StructMem:
				return Gen((StructMemNode) node);
			case NodeKind.VarMem:
				return Gen((VarMemNode) node, extrn);

			case NodeKind.BlockStmt:
				return Gen((BlockStmtNode) node);
			case NodeKind.IfStmt:
				return Gen((IfStmtNode) node);
			case NodeKind.RetStmt:
				return Gen((RetStmtNode) node);
			case NodeKind.TypedefStmt:
				return Gen((TypedefStmtNode) node);
			case NodeKind.VarStmt:
				return Gen((VarStmtNode) node);
			case NodeKind.WhileStmt:
				return Gen((WhileStmtNode) node);

			case NodeKind.ArrExpr:
				return Gen((ArrExprNode) node);
			case NodeKind.AssignExpr:
				return Gen((AssignExprNode) node);
			case NodeKind.BinExpr:
				return Gen((BinExprNode) node);
			case NodeKind.CallExpr:
				return Gen((CallExprNode) node);
			case NodeKind.CastExpr:
				return Gen((CastExprNode) node);
			case NodeKind.FNumExpr:
				return Gen((FNumExprNode) node);
			case NodeKind.MemExpr:
				return Gen((MemExprNode) node);
			case NodeKind.NumExpr:
				return Gen((NumExprNode) node);
			case NodeKind.StrExpr:
				return Gen((StrExprNode) node);
			case NodeKind.SymExpr:
				return Gen((SymExprNode) node);
			case NodeKind.TypePunExpr:
				return Gen((TypePunExprNode) node);
			case NodeKind.UnExpr:
				return Gen((UnExprNode) node);

			default:
				throw new InvalidOperationException("Invalid node");
			}
		}

		private LLVMValueRef Gen(DeclFuncMemNode node)
		{
			if (node.impl is not null)
				return null;
			var fn = module.AddFunction(node.name, node.type);
			syms.Add(node, fn);
			return fn;
		}

		private LLVMValueRef Gen(ImportMemNode node)
		{
			foreach (var mem in node.module.mems)
				Gen(mem, true);
			return null;
		}

		private LLVMValueRef Gen(FuncMemNode node, bool extrn = false)
		{
			func = module.AddFunction(node.name, node.type);
			syms.Add(node, func);
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


		private LLVMValueRef Gen(ArrExprNode node)
		{
			var elements = new LLVMValueRef[node.elements.Length];
			var arr = builder.BuildAlloca(LLVMTypeRef.CreateArray(node.type.ElementType, (uint)elements.Length), "..arr");
			var ptr = builder.BuildBitCast(arr, node.type);

			for (int i = 0; i < elements.Length; i++)
			{
				var index = builder.BuildInBoundsGEP(ptr, new LLVMValueRef[] { LLVMValueRef.CreateConstInt(LLVMTypeRef.Int64, (ulong)i) });
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

			return builder.BuildCall(what, args);
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

		private LLVMValueRef Gen(MemExprNode node)
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
			var fptr = builder.BuildAlloca(expr.TypeOf);
			builder.BuildStore(expr, fptr);
			var tptr = builder.BuildPointerCast(fptr, node.type.Ptr());
			return builder.BuildLoad(tptr);
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


		private LLVMValueRef GenAddr(Node? node)
		{
			if (node is null)
				return LLVMValueRef.CreateConstNull(LLVMTypeRef.Void);

			switch (node.kind)
			{
			case NodeKind.MemExpr:
				return GenAddr((MemExprNode) node);
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

		private LLVMValueRef GenAddr(MemExprNode node)
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
