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

			var module = LLVMModuleRef.CreateWithName("SympleCode");
			var builder = LLVMBuilderRef.Create(module.Context);

			var funcTy = LLVMTypeRef.CreateFunction(LLVMTypeRef.Int32, Array.Empty<LLVMTypeRef>());
			var func = module.AddFunction("Main", funcTy);
			var entry = func.AppendBasicBlock("Entry");

			builder.PositionAtEnd(entry);
			var val = tree.CodeGen(builder);
			var fptr = builder.BuildAlloca(LLVMTypeRef.Float);
			builder.BuildStore(val, fptr);
			var iptr = builder.BuildBitCast(fptr, LLVMTypeRef.CreatePointer(LLVMTypeRef.Int32, 0));
			var ret = builder.BuildLoad(iptr);
			builder.BuildRet(ret);

			Console.ForegroundColor = ConsoleColor.DarkYellow;
			Console.WriteLine(func);

			LLVM.LinkInMCJIT();
			LLVM.InitializeAllTargetMCs();
			LLVM.InitializeAllTargets();
			LLVM.InitializeAllTargetInfos();
			LLVM.InitializeAllAsmParsers();
			LLVM.InitializeAllAsmPrinters();

			if (!module.TryVerify(LLVMVerifierFailureAction.LLVMPrintMessageAction, out string err))
				Console.Error.WriteLine(err);

			var options = new LLVMMCJITCompilerOptions { NoFramePointerElim = 1 };
			if (!module.TryCreateMCJITCompiler(out var engine, ref options, out err))
				Console.Error.WriteLine(err);

			var program = (Run)Marshal.GetDelegateForFunctionPointer(engine.GetPointerToGlobal(func), typeof(Run));
			var result = new FIUnion { ival = program() };

			Console.ForegroundColor = ConsoleColor.Gray;
			Console.WriteLine($"Returned {result.fval}");
			Console.ReadKey();
		}
	}
}
