//#define SYNTAX_ONLY

using System;
using System.Runtime.InteropServices;
using LLVMSharp.Interop;

namespace SuperCode
{
	public class Program
	{
		[UnmanagedFunctionPointer(CallingConvention.Cdecl)]
		public delegate void Run();

		private static void Main(string[] _)
		{
			var parser = new Parser("Main.sy");
			Console.ForegroundColor = ConsoleColor.DarkGreen;
			foreach (var tok in parser.tokens)
				Console.WriteLine(tok);
			Console.WriteLine();

			var tree = parser.Parse();
			tree.Print(Console.Out);
			Console.WriteLine();

#if !SYNTAX_ONLY

			var noder = new Noder(tree);
			var node = noder.Nodify();

			var module = LLVMModuleRef.CreateWithName("SympleCode");
			var builder = LLVMBuilderRef.Create(module.Context);
			node.Build(module, builder);

			Console.ForegroundColor = ConsoleColor.DarkGreen;
			Console.WriteLine(module);

			Console.ForegroundColor = ConsoleColor.White;
			RunJIT(module);
#endif
			Console.ReadKey();
		}

#if !SYNTAX_ONLY
		private static void RunJIT(LLVMModuleRef module)
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

			var exec = (Run) Marshal.GetDelegateForFunctionPointer(engine.GetPointerToGlobal(module.GetNamedFunction("Run")), typeof(Run));
			exec();
		}
#endif
	}
}
