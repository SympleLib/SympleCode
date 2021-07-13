using System;
using System.Runtime.InteropServices;
using LLVMSharp.Interop;

namespace SuperCode
{
	[StructLayout(LayoutKind.Explicit)]
	struct FIUnion
	{
		[FieldOffset(0)]
		public float fval;
		[FieldOffset(0)]
		public int ival;
	}

	public class Program
	{
		[UnmanagedFunctionPointer(CallingConvention.Cdecl)]
		public delegate int Run();

		private static void Main(string[] _)
		{
			var parser = new Parser("Main.sy");
			var tree = parser.Parse();
			tree.Print(Console.Out);
			Console.WriteLine();

			var emitter = new Emitter(tree);
			var module = emitter.Emit();

			Console.ForegroundColor = ConsoleColor.DarkGreen;
			Console.WriteLine(emitter.mainFn);

			var result = new FIUnion { ival = RunJIT(module, emitter.runFn) };
			Console.ForegroundColor = ConsoleColor.White;
			Console.WriteLine($"Returned {result.fval}");
			Console.ReadKey();
		}

		private static int RunJIT(LLVMModuleRef module, LLVMValueRef runFn)
		{
			LLVM.LinkInMCJIT();
			LLVM.InitializeNativeTarget();
			LLVM.InitializeNativeAsmParser();
			LLVM.InitializeNativeAsmPrinter();
			LLVM.InitializeNativeDisassembler();

			if (!module.TryVerify(LLVMVerifierFailureAction.LLVMPrintMessageAction, out string err))
				Console.Error.WriteLine(err);

			var options = new LLVMMCJITCompilerOptions { NoFramePointerElim = 1 };
			if (!module.TryCreateMCJITCompiler(out var engine, ref options, out err))
				Console.Error.WriteLine(err);

			var exec = (Run) Marshal.GetDelegateForFunctionPointer(engine.GetPointerToGlobal(runFn), typeof(Run));
			return exec();
		}
	}
}
