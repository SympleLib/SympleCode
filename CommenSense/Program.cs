global using LLVMSharp.Interop;

global using static LLVMSharp.Interop.LLVMOpcode;

using CommenSense;

const string src = "1 * 2 * 3 * 4 * 5";

Parser parser = new Parser(src);
Ast ast = parser.Parse();
Console.WriteLine(ast);

Console.WriteLine("---");

ast = Resolver.Resolve(ast);
Console.WriteLine(ast);

Console.WriteLine("---");

Builder builder = new Builder(ast);
LLVMValueRef val = builder.BuildExpr((ExprAst) ast);
Console.WriteLine(val.ConstIntSExt);
Console.ReadKey();