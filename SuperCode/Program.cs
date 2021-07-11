using System;
using LLVMSharp.Interop;

namespace SuperCode
{
	public class Program
	{
		private static void Main(string[] args)
		{
			LLVMModuleRef module = LLVMModuleRef.CreateWithName("SympleCode");
			LLVMBuilderRef builder = LLVMBuilderRef.Create(module.Context);
			
			LLVMTypeRef[] paramz = new LLVMTypeRef[2] { LLVMTypeRef.Int32, LLVMTypeRef.Int32 };
			LLVMTypeRef ft = LLVMTypeRef.CreateFunction(LLVMTypeRef.Int32, paramz);
			LLVMValueRef fn = module.AddFunction("Add", ft);

			LLVMBasicBlockRef entry = fn.AppendBasicBlock("Entry");
			builder.PositionAtEnd(entry);
			LLVMValueRef ret = builder.BuildAdd(fn.Params[0], fn.Params[1]);
			builder.BuildRet(ret);


			Console.WriteLine(fn.PrintToString());
			Console.ReadKey();
		}
	}
}
