//#define SYNTAX_ONLY

using System;
using System.Reflection;
using System.IO;
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

		private static void Main(string[] _)
		{
			//Console.Write("Input file: ");
			//string file = Console.ReadLine();
			const string dir = "../Examples/";
			const string file = "Code.sy";
			var syc = new SympleCode((SycMode.Dev) & ~SycMode.Optimize, dir);
			var module = syc.CompileJIT(file);
			if (module is null)
				goto Stop;

			RunJIT(syc, module.llmodule);
		Stop:
			Console.ReadKey();
		}

#if !SYNTAX_ONLY
		private static void RunJIT(SympleCode syc, LLVMModuleRef module)
		{
			var func = module.GetNamedFunction("run");
			if (func == null)
				return;
			var exec = (Run) Marshal.GetDelegateForFunctionPointer(syc.execEngine.GetPointerToGlobal(func), typeof(Run));
			exec();
		}
#endif
	}
}
