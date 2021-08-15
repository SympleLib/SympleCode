using System;
using System.Runtime.InteropServices;
using LLVMSharp.Interop;

namespace SuperCode
{
	public class Program
	{
		[UnmanagedFunctionPointer(CallingConvention.Cdecl)]
		public delegate void Run();

		[StructLayout(LayoutKind.Explicit)]
		private unsafe struct FPIUnion
		{
			[FieldOffset(0)]
			public float fval;
			[FieldOffset(0)]
			public int ival;

			public override string ToString() =>
				$"Int: {ival}, Float: {fval}";
		}

		private static void Main(string[] args)
		{
			string path = "../Examples/Test.sy";
			string dir = path[..path.LastIndexOf('/')] + '/';
			string file = path[(path.LastIndexOf('/') + 1)..];

			var syc = new SympleCode(SycMode.Dev & ~SycMode.Optimize, dir);
			var module = syc.CompileJIT(file);
			if (module is null)
				goto Stop;

			RunJIT(syc);
		Stop:
			Console.ReadKey();
		}

		private static void RunJIT(SympleCode syc)
		{
			var runFn = syc.execEngine.FindFunction("run");
			if (runFn == null)
			{
				Console.ForegroundColor = ConsoleColor.Red;
				Console.WriteLine("'run' Function not found");
				return;
			}

			var module = LLVMModuleRef.CreateWithName("^_^");
			var paramz = new LLVMTypeRef[] { LLVMTypeRef.Int32, LLVMTypeRef.Int8.Ptr().Ptr() };
			var fnTy = LLVMTypeRef.CreateFunction(LLVMTypeRef.Int32, paramz);
			var func = module.AddFunction("^_^", fnTy);
			var builder = LLVMBuilderRef.Create(module.Context);
			var entry = func.AppendBasicBlock();
			builder.PositionAtEnd(entry);
			builder.BuildCall(runFn, Array.Empty<LLVMValueRef>());
			builder.BuildUnreachable();
			syc.execEngine.AddModule(module);

			Console.ForegroundColor = ConsoleColor.DarkGray;
			Console.WriteLine("Running...");
			Console.ForegroundColor = ConsoleColor.White;
			int result = syc.execEngine.RunFunctionAsMain(func, 0, Array.Empty<string>(), Array.Empty<string>());
			Console.ForegroundColor = ConsoleColor.Cyan;
			Console.WriteLine($"Program returned {result}");
		}
	}
}
