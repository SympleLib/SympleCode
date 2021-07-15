#define SYNTAX_ONLY

using System;
using System.Runtime.InteropServices;
using LLVMSharp.Interop;

namespace SuperCode
{
	public class Program
	{
		[UnmanagedFunctionPointer(CallingConvention.Cdecl)]
		public delegate int Run();

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
			var mainFn = BuildMain(module, builder, node);

			Console.ForegroundColor = ConsoleColor.DarkGreen;
			Console.WriteLine(module);

			int result = RunJIT(module, mainFn);
			Console.ForegroundColor = ConsoleColor.White;
			Console.WriteLine($"Returned {result}");
#endif
			Console.ReadKey();
		}

#if !SYNTAX_ONLY
		private static LLVMValueRef BuildMain(LLVMModuleRef module, LLVMBuilderRef builder, Node node)
		{
			var fnTy = LLVMTypeRef.CreateFunction(LLVMTypeRef.Int32, Array.Empty<LLVMTypeRef>());
			var fn = module.AddFunction("Main", fnTy);
			var entry = fn.AppendBasicBlock("Entry");
			builder.PositionAtEnd(entry);
			node.Build(module, builder);
			return fn;
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
#endif
	}
}
