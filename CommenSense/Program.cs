global using LLVMSharp;
global using LLVMSharp.Interop;

global using static LLVMSharp.Interop.LLVMOpcode;
global using static LLVMSharp.Interop.LLVMVisibility;

using System.Runtime.InteropServices;

using CommenSense;

const string src = @"
decl void puts[char* str]

void run {
	puts['Hello, world!']
}
";

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
	goto End;
}

LLVM.LinkInMCJIT();

LLVM.InitializeX86TargetMC();
LLVM.InitializeX86Target();
LLVM.InitializeX86TargetInfo();
LLVM.InitializeX86AsmParser();
LLVM.InitializeX86AsmPrinter();

LLVMMCJITCompilerOptions options = new LLVMMCJITCompilerOptions { NoFramePointerElim = 1 };
if (!llModule.TryCreateMCJITCompiler(out LLVMExecutionEngineRef engine, ref options, out error))
{
	Console.WriteLine($"Error: {error}");
	goto End;
}

var runFn = (Run) Marshal.GetDelegateForFunctionPointer(engine.GetPointerToGlobal(llModule.GetNamedFunction("run")), typeof(Run));
runFn();

End:
Console.WriteLine("---");
Console.ReadKey();


[UnmanagedFunctionPointer(CallingConvention.Cdecl)]
delegate void Run();