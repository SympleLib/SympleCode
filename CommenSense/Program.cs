﻿global using LLVMSharp.Interop;

global using static LLVMSharp.Interop.LLVMOpcode;
global using static LLVMSharp.Interop.LLVMVisibility;

using CommenSense;

const string src = @"
void run {
	void run {
		void run {
			bool inception = true
		}
	}
}
";

Parser parser = new Parser(src);
ModuleAst ast = parser.Parse();
Console.WriteLine(ast);

Console.WriteLine("---");

Builder builder = new Builder(ast);
LLVMModuleRef llModule = builder.Build();
Console.WriteLine(llModule);
Console.ReadKey();