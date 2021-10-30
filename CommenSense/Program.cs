﻿global using LLVMSharp;
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
using System.Reflection;

// I wont use this buddy for debugging
#pragma warning disable CS8321 // Local function is declared but never used
void Optimize(LLVMModuleRef module)
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

/*
LLVMExecutionEngineRef? CompileMulti(string filename, params string[] filenames)
{
	LLVMModuleRef llModule;
	LLVMModuleRef[] llMods = new LLVMModuleRef[filenames.Length];

	ModuleAst ast = ParseSingle(filename);
	ModuleAst[] asts = new ModuleAst[filenames.Length];
	for (int i = 0; i < filenames.Length; i++)
		asts[i] = ParseSingle(filenames[i]);


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
*/


LLVMExecutionEngineRef? Compile(string filename)
{
	// pre-parse
	{
		string src = File.ReadAllText(filename);
		Parser parser = new Parser(src, filename);
		parser.PreParse();
	}

	// parse
	ModuleAst[] modules;
	{
		List<ModuleAst> moduleList = new List<ModuleAst>();
		foreach (Parser parser in Parser.parsers.Values)
			moduleList.Add(parser.Parse());
		modules = moduleList.ToArray();
	}

	// build
	List<LLVMModuleRef> llModules = new List<LLVMModuleRef>();
	for (int i = 0; i < modules.Length; i++)
	{
		Builder builder = new Builder(modules, i);

		LLVMModuleRef llModule = builder.Build();
#if false // No need for the Infini-Mizing (The tiny program will run too fast, too much power ⚡ for mere mortals to handle)
	//      👇 To insure COMPLETE optimization
	while (true)
#endif
		// Optimize(llModule);
		Console.WriteLine(llModule);

		Console.WriteLine("---");
		if (!llModule.TryVerify(LLVMVerifierFailureAction.LLVMPrintMessageAction, out string err))
		{
			Console.WriteLine($"Error: {err}");
			Console.WriteLine("---");
			return null;
		}

		llModules.Add(llModule);
	}

	LLVM.LinkInMCJIT();
	LLVM.InitializeNativeTarget();
	LLVM.InitializeNativeAsmPrinter();

	LLVMMCJITCompilerOptions options = new LLVMMCJITCompilerOptions { NoFramePointerElim = 1 };
	if (!llModules[0].TryCreateMCJITCompiler(out LLVMExecutionEngineRef engine, ref options, out string error))
	{
		Console.WriteLine($"Error: {error}");
		return null;
	}

	foreach (LLVMModuleRef llModule in llModules)
		engine.AddModule(llModule);

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