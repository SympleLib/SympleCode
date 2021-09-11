global using LLVMSharp.Interop;

global using static LLVMSharp.Interop.LLVMOpcode;

using CommenSense;

const string src = @"
1 * 2 * 3 * 4 * 5
6 * 9
17
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