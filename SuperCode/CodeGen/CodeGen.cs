using System;
using System.Collections.Generic;

using LLVMSharp.Interop;

namespace SuperCode
{
	public partial class CodeGen
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
				return Gen((FieldExprNode) node);
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
	}
}
