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

LLVMModuleRef llMod1, llMod2;

{
	string src = File.ReadAllText("samples/test.sy");
	Parser parser = new Parser(src);
	ModuleAst ast = parser.Parse();
	Console.WriteLine(ast);

	Console.WriteLine("---");

	Builder builder = new Builder(ast);
	llMod1 = builder.Build();
	Console.WriteLine(llMod1);

	Console.WriteLine("---");
}

{
	string src = File.ReadAllText("samples/test2.sy");
	Parser parser = new Parser(src);
	ModuleAst ast = parser.Parse();
	Console.WriteLine(ast);

	Console.WriteLine("---");

	Builder builder = new Builder(ast);
	llMod2 = builder.Build();
	Console.WriteLine(llMod2);

	Console.WriteLine("---");
}

if (!llMod1.TryVerify(LLVMVerifierFailureAction.LLVMPrintMessageAction, out string error))
{
	Console.WriteLine($"Error: {error}");
	goto End;
}

if (!llMod2.TryVerify(LLVMVerifierFailureAction.LLVMPrintMessageAction, out error))
{
	Console.WriteLine($"Error: {error}");
	goto End;
}

LLVM.LinkInMCJIT();

LLVM.InitializeX86TargetMC();
LLVM.InitializeX86Target();
LLVM.InitializeX86TargetInfo();
LLVM.InitializeX86AsmParser();
LLVM.InitializeX86AsmPrinter();

LLVMMCJITCompilerOptions options = new LLVMMCJITCompilerOptions { NoFramePointerElim = 1 };
if (!llMod1.TryCreateMCJITCompiler(out LLVMExecutionEngineRef engine, ref options, out error))
{
	Console.WriteLine($"Error: {error}");
	goto End;
}

engine.AddModule(llMod2);

var runFn = (Run) Marshal.GetDelegateForFunctionPointer(engine.GetPointerToGlobal(llMod1.GetNamedFunction("run")), typeof(Run));
runFn();

End:
Console.WriteLine("---");
Console.ReadKey();


[UnmanagedFunctionPointer(CallingConvention.Cdecl)]
delegate void Run();