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
			var expr = parser.Parse();
			expr.Print(Console.Out);
			Console.WriteLine();

			var module = LLVMModuleRef.CreateWithName("SympleCode");
			var builder = LLVMBuilderRef.Create(module.Context);

			var funcTy = LLVMTypeRef.CreateFunction(LLVMTypeRef.Int32, Array.Empty<LLVMTypeRef>());
			var func = module.AddFunction("Main", funcTy);
			var entry = func.AppendBasicBlock("Entry");

			builder.PositionAtEnd(entry);
			var val = expr.CodeGen(builder);
			builder.BuildRet(val);

			Console.ForegroundColor = ConsoleColor.DarkYellow;
			Console.WriteLine(func);

			LLVM.LinkInMCJIT();
			LLVM.InitializeX86TargetMC();
			LLVM.InitializeX86Target();
			LLVM.InitializeX86TargetInfo();
			LLVM.InitializeX86AsmParser();
			LLVM.InitializeX86AsmPrinter();

			if (!module.TryVerify(LLVMVerifierFailureAction.LLVMPrintMessageAction, out string err))
				Console.Error.WriteLine(err);

			var options = new LLVMMCJITCompilerOptions { NoFramePointerElim = 1 };
			if (!module.TryCreateMCJITCompiler(out var engine, ref options, out err))
				Console.Error.WriteLine(err);

			var program = (Run)Marshal.GetDelegateForFunctionPointer(engine.GetPointerToGlobal(func), typeof(Run));
			int result = program();

			Console.ForegroundColor = ConsoleColor.Gray;
			Console.WriteLine($"Returned {result}");
			Console.ReadKey();
		}
	}
}
