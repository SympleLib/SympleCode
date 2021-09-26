global using LLVMSharp;
global using LLVMSharp.Interop;

global using System;
global using System.Collections.Generic;
global using System.Linq;

global using static LLVMSharp.Interop.LLVMOpcode;
global using static LLVMSharp.Interop.LLVMVisibility;

using System.IO;
using System.Runtime.InteropServices;

using CommenSense;

LLVMModuleRef? CompileSingle(string path)
{
	string src = File.ReadAllText(path);
	Parser parser = new Parser(src);
	ModuleAst ast = parser.Parse();
	Console.WriteLine(ast);

	Console.WriteLine("---");

	Builder builder = new Builder(ast);
	LLVMModuleRef llModule = builder.Build();
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

	LLVMModuleRef? tmp = CompileSingle(path);
	if (tmp is null)
		return null;

	llModule = tmp.Value;

	for (int i = 0; i < paths.Length; i++)
	{
		tmp = CompileSingle(paths[i]);
		if (tmp is null)
			return null;

		llMods[i] = tmp.Value;
	}

	LLVM.LinkInMCJIT();

	LLVM.InitializeX86TargetMC();
	LLVM.InitializeX86Target();
	LLVM.InitializeX86TargetInfo();
	LLVM.InitializeX86AsmParser();
	LLVM.InitializeX86AsmPrinter();

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

LLVMExecutionEngineRef? engine = Compile("samples/test.sy");
if (engine is null)
	goto End;

var runFn = (Run) Marshal.GetDelegateForFunctionPointer((IntPtr) engine?.GetFunctionAddress("run"), typeof(Run));
runFn();

End:
Console.WriteLine("---");
Console.ReadKey();


[UnmanagedFunctionPointer(CallingConvention.Cdecl)]
delegate void Run();