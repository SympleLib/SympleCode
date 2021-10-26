global using LLVMSharp;
global using LLVMSharp.Interop;

global using System;
global using System.Collections.Generic;
global using System.Linq;

global using static LLVMSharp.Interop.LLVMOpcode;
global using static LLVMSharp.Interop.LLVMVisibility;
global using static LLVMSharp.Interop.LLVMCallConv;

global using Visibility = LLVMSharp.Interop.LLVMVisibility;
global using CallConv = LLVMSharp.Interop.LLVMCallConv;

using System.IO;
using System.Runtime.InteropServices;

using CommenSense;

// I wont use this buddy for debugging
#pragma warning disable CS8321 // Local function is declared but never used
void Optimize(LLVMModuleRef module)
#pragma warning restore CS8321 // Local function is declared but never used
{
	LLVMPassManagerRef pass = LLVMPassManagerRef.Create();
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

ModuleAst ParseSingle(string path)
{
	string src = File.ReadAllText(path);
	Parser parser = new Parser(src);
	return parser.Parse();
}

LLVMModuleRef? BuildSingle(Builder builder)
{
	LLVMModuleRef llModule = builder.Build();
#if false // No need for the Infini-Mizing (The tiny program will run too fast, too much power ⚡ for mere mortals to handle)
	//      👇 To insure COMPLETE optimization
	while (true)
#endif
	//Optimize(llModule);
	Console.WriteLine(llModule);

	Console.WriteLine("---");
	if (!llModule.TryVerify(LLVMVerifierFailureAction.LLVMPrintMessageAction, out string error))
	{
		Console.WriteLine($"Error: {error}");
		Console.WriteLine("---");
		return null;
	}

	return llModule;
}

LLVMExecutionEngineRef? Compile(string path, params string[] paths)
{
	LLVMModuleRef llModule;
	LLVMModuleRef[] llMods = new LLVMModuleRef[paths.Length];

	ModuleAst ast = ParseSingle(path);
	ModuleAst[] asts = new ModuleAst[paths.Length];
	for (int i = 0; i < paths.Length; i++)
		asts[i] = ParseSingle(paths[i]);


	Builder builder = new Builder(ast);
	builder.Link(asts);
	LLVMModuleRef? tmp = BuildSingle(builder);
	if (tmp is null)
		return null;

	llModule = tmp.Value;

	for (int i = 0; i < asts.Length; i++)
	{
		builder = new Builder(asts[i]);
		builder.Link(ast);

		for (int j = 0; j < asts.Length; j++)
			if (j != i)
				builder.Link(asts);

		tmp = BuildSingle(builder);
		if (tmp is null)
			return null;

		llMods[i] = tmp.Value;
	}

	LLVM.LinkInMCJIT();
	LLVM.InitializeNativeTarget();
	LLVM.InitializeNativeAsmPrinter();

	LLVMMCJITCompilerOptions options = new LLVMMCJITCompilerOptions { NoFramePointerElim = 1 };
	if (!llModule.TryCreateMCJITCompiler(out LLVMExecutionEngineRef engine, ref options, out string error))
	{
		Console.WriteLine($"Error: {error}");
		return null;
	}

	foreach (LLVMModuleRef llMod in llMods)
		engine.AddModule(llMod);

	return engine;
}

LLVMExecutionEngineRef? _engine = Compile("samples/test.sy");
if (_engine is null)
	goto End;

var engine = _engine.Value;
var runFn = (Run) Marshal.GetDelegateForFunctionPointer((IntPtr) engine.GetFunctionAddress("run"), typeof(Run));
runFn();

End:
Console.WriteLine("---");
Console.ReadKey();


[UnmanagedFunctionPointer(CallingConvention.Cdecl)]
delegate void Run();