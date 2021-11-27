global using LLVMSharp;
global using LLVMSharp.Interop;

global using System;
global using System.Collections.Generic;
global using System.Linq;

global using static LLVMSharp.Interop.LLVMOpcode;
global using static LLVMSharp.Interop.LLVMVisibility;
global using static LLVMSharp.Interop.LLVMCallConv;
global using static LLVMSharp.Interop.LLVMTypeKind;
global using static LLVMSharp.Interop.LLVMIntPredicate;

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

void Compile(string filename)
{
	if (!File.Exists(filename))
	{
		Console.WriteLine($"file '{filename}' does not exist");
		return;
	}

	Parser.parsers.Clear();

	// pre-parse
	{
		string src = File.ReadAllText(filename);
		Parser parser = new Parser(src, filename);
		if (BadCode.errors.Count > 0)
		{
			foreach (SyntaxError err in BadCode.errors)
				Console.WriteLine(err);
			return;
		}

		parser.PreParse();

		if (BadCode.errors.Count > 0)
		{
			foreach (SyntaxError err in BadCode.errors)
				Console.WriteLine(err);
			return;
		}
	}

	// parse
	ModuleAst[] modules;
	{
		List<ModuleAst> moduleList = new List<ModuleAst>();
		foreach (Parser parser in Parser.parsers.Values)
		{
			ModuleAst module = parser.Parse();
			moduleList.Add(module);
		}

		if (BadCode.errors.Count > 0)
		{
			foreach (SyntaxError err in BadCode.errors)
				Console.WriteLine(err);
			return;
		}

		modules = moduleList.ToArray();
	}

	// build
	List<LLVMModuleRef> llModules = new List<LLVMModuleRef>();
	for (int i = 0; i < modules.Length; i++)
	{
		Builder builder = new Builder(modules, i);

		LLVMModuleRef llModule = builder.Build();
		if (BadCode.errors.Count > 0)
		{
			foreach (SyntaxError erronius in BadCode.errors)
				Console.WriteLine(erronius);
			return;
		}

#if false // No need for the Infini-Mizing (The tiny program will run too fast, too much power ⚡ for mere mortals to handle)
	//      👇 To insure COMPLETE optimization
	while (true)
#endif
		// Optimize(llModule);

		if (!llModule.TryVerify(LLVMVerifierFailureAction.LLVMPrintMessageAction, out string err))
			return;

		llModules.Add(llModule);
	}

	LLVM.InitializeAllTargetInfos();
	LLVM.InitializeAllTargets();
	LLVM.InitializeAllTargetMCs();
	LLVM.InitializeAllAsmParsers();
	LLVM.InitializeAllAsmPrinters();

	LLVMTargetRef target = LLVMTargetRef.GetTargetFromTriple(LLVMTargetRef.DefaultTriple);
	LLVMTargetMachineRef machine = target.CreateTargetMachine(LLVMTargetRef.DefaultTriple, "generic", "",
				LLVMCodeGenOptLevel.LLVMCodeGenLevelAggressive, LLVMRelocMode.LLVMRelocDefault, LLVMCodeModel.LLVMCodeModelDefault);
	for (int i = 0; i < llModules.Count; i++)
		machine.EmitToFile(llModules[i], modules[i].name[..modules[i].name.LastIndexOf('.')] + ".o", LLVMCodeGenFileType.LLVMObjectFile);
}

LLVMExecutionEngineRef? Debug(string filename)
{
	using StreamWriter dbgout = new StreamWriter(File.OpenWrite("dbgout.txt"));
	Parser.parsers.Clear();

	// pre-parse
	{
		string src = File.ReadAllText(filename);
		Parser parser = new Parser(src, filename);
		if (BadCode.errors.Count > 0)
		{
			foreach (SyntaxError err in BadCode.errors)
			{
				Console.WriteLine(err);
				dbgout.WriteLine(err);
			}
			return null;
		}

		parser.PreParse();

		if (BadCode.errors.Count > 0)
		{
			foreach (SyntaxError err in BadCode.errors)
			{
				Console.WriteLine(err);
				dbgout.WriteLine(err);
			}
			return null;
		}
	}

	// parse
	ModuleAst[] modules;
	{
		List<ModuleAst> moduleList = new List<ModuleAst>();
		foreach (Parser parser in Parser.parsers.Values)
		{
			ModuleAst module = parser.Parse();
			dbgout.WriteLine(module);
			dbgout.WriteLine("---");
			moduleList.Add(module);
		}

		dbgout.Flush();

		if (BadCode.errors.Count > 0)
		{
			foreach (SyntaxError err in BadCode.errors)
			{
				Console.WriteLine(err);
				dbgout.WriteLine(err);
			}
			return null;
		}

		modules = moduleList.ToArray();
	}

	// build
	List<LLVMModuleRef> llModules = new List<LLVMModuleRef>();
	for (int i = 0; i < modules.Length; i++)
	{
		Builder builder = new Builder(modules, i);

		LLVMModuleRef llModule = builder.Build();
		if (BadCode.errors.Count > 0)
		{
			foreach (SyntaxError erronius in BadCode.errors)
			{
				Console.WriteLine(erronius);
				dbgout.WriteLine(erronius);
			}
			return null;
		}

#if false // No need for the Infini-Mizing (The tiny program will run too fast, too much power ⚡ for mere mortals to handle)
	//      👇 To insure COMPLETE optimization
	while (true)
#endif
		// Optimize(llModule);

		dbgout.WriteLine(llModule);

		if (!llModule.TryVerify(LLVMVerifierFailureAction.LLVMPrintMessageAction, out string err))
		{
			dbgout.WriteLine("---");
			dbgout.WriteLine(err);
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
		Console.WriteLine(error);
		dbgout.WriteLine(error);
		return null;
	}

	foreach (LLVMModuleRef llModule in llModules)
		engine.AddModule(llModule);

	return engine;
}

#if RELEASE
string filename;
if (args.Length > 0)
	filename = args[0];
else
{
	Console.WriteLine("type in the name of file you want to compile");
	string? tmp = Console.ReadLine();
	if (string.IsNullOrEmpty(tmp))
	{
		Console.WriteLine("invalid response, aborting...");
		return;
	}

	filename = tmp;
}

Console.WriteLine($"compiling '{filename}' -> '{filename[..filename.LastIndexOf('.')] + ".o"}'...");
Compile(filename);
#elif DEBUG
string filename = "samples/test.sy";
LLVMExecutionEngineRef? _engine = Debug(filename);
if (_engine is null)
	return;

var engine = _engine.Value;
var runFn = (Run) Marshal.GetDelegateForFunctionPointer((IntPtr) engine.GetFunctionAddress("run"), typeof(Run));
runFn();

[UnmanagedFunctionPointer(CallingConvention.Cdecl)]
delegate void Run();
#endif