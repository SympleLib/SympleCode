global using LLVMSharp.Interop;

global using static LLVMSharp.Interop.LLVMOpcode;
global using static LLVMSharp.Interop.LLVMVisibility;

using CommenSense;

const string src = @"
void run {
	6 * 9
	69 + 420 + 1337
	1 + 2 + 3 + 4 + 5
	1 * 2 * 3 * 4 * 5
}
";

Parser parser = new Parser(src);
ModuleAst ast = parser.Parse();
Console.WriteLine(ast);

Console.WriteLine("---");

//ast = Resolver.Resolve(ast);
//Console.WriteLine(ast);

//Console.WriteLine("---");

Builder builder = new Builder(ast);
LLVMModuleRef llModule = builder.Build();
Console.WriteLine(llModule);
Console.ReadKey();