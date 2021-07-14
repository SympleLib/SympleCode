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
#pragma warning disable CS0162 // Unreachable code detected
			var parser = new Parser("Main.sy");
			var tree = parser.Parse();
			tree.Print(Console.Out);
			Console.WriteLine();
			goto Wait;

			var noder = new Noder(tree);
			var node = noder.Nodify();

			var module = LLVMModuleRef.CreateWithName("SympleCode");
			var builder = LLVMBuilderRef.Create(module.Context);
			var mainFn = BuildMain(module, builder, node);

			Console.ForegroundColor = ConsoleColor.DarkGreen;
			Console.WriteLine(mainFn);

			int result = RunJIT(module, mainFn);
			Console.ForegroundColor = ConsoleColor.White;
			Console.WriteLine($"Returned {result}");

#pragma warning restore CS0162 // Unreachable code detected
		Wait:
			Console.ReadKey();
		}

		private static LLVMValueRef BuildMain(LLVMModuleRef module, LLVMBuilderRef builder, Node node)
		{
			var fnTy = LLVMTypeRef.CreateFunction(LLVMTypeRef.Int32, Array.Empty<LLVMTypeRef>());
			var fn = module.AddFunction("Main", fnTy);
			var entry = fn.AppendBasicBlock("Entry");
			builder.PositionAtEnd(entry);
			var ret = node.Build(builder);
			builder.BuildRet(ret);
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
	}
}
