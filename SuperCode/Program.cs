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
			string path = "../Examples/Tic Tac Toe.sy";
			string dir = path[..path.LastIndexOf('/')] + '/';
			string file = path[(path.LastIndexOf('/') + 1)..];

			var syc = new SympleCode(SycMode.Dev, dir);
			var module = syc.CompileJIT(file);
			if (module is null)
				goto Stop;

			RunJIT(syc, module.llmodule);
		Stop:
			Console.ReadKey();
		}

		private static void RunJIT(SympleCode syc, LLVMModuleRef module)
		{
			var func = module.GetNamedFunction("run");
			if (func == null)
				return;
			var exec = (Run) Marshal.GetDelegateForFunctionPointer(syc.execEngine.GetPointerToGlobal(func), typeof(Run));
			Console.ForegroundColor = ConsoleColor.DarkGray;
			Console.WriteLine("Running...");
			Console.ForegroundColor = ConsoleColor.White;
			exec();
		}
	}
}
