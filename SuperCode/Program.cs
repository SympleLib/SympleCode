using System;
using LLVMSharp.Interop;

namespace SuperCode
{
	public class Program
	{
		private static void Main(string[] _args)
		{
			var parser = new Parser("Main.sy");

			Console.ForegroundColor = ConsoleColor.Yellow;
			foreach (var token in parser.tokens)
				Console.WriteLine(token);
			Console.WriteLine();

			var expr = parser.Parse();

			var module = LLVMModuleRef.CreateWithName("SympleCode");
			var builder = LLVMBuilderRef.Create(module.Context);

			var funcTy = LLVMTypeRef.CreateFunction(LLVMTypeRef.Int32, new LLVMTypeRef[] { });
			var func = module.AddFunction("Main", funcTy);
			var entry = func.AppendBasicBlock("Entry");

			builder.PositionAtEnd(entry);
			var val = expr.CodeGen(builder);
			builder.BuildRet(val);

			Console.ForegroundColor = ConsoleColor.Green;
			Console.WriteLine(func);
			Console.ReadKey();
		}
	}
}
