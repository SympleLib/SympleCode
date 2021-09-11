global using LLVMSharp.Interop;

global using static LLVMSharp.Interop.LLVMOpcode;

using CommenSense;

const string src = "69 * 420";

Parser parser = new Parser(src);
Ast ast = parser.Parse();
Console.WriteLine(ast);

Console.WriteLine("---");

ast = Resolver.Resolve(ast);
Console.WriteLine(ast);

Console.WriteLine("---");

using LLVMBuilderRef builder = LLVMBuilderRef.Create(LLVMContextRef.Global);
LLVMValueRef val = builder.Build((ExprAst) ast);
Console.WriteLine(val.ConstIntSExt);
Console.ReadKey();

static class _
{
	public static LLVMValueRef Build(this LLVMBuilderRef builder, ExprAst expr)
	{
		if (expr is IntLiteralExprAst intLiteral)
			return LLVMValueRef.CreateConstInt(LLVMTypeRef.Int32, intLiteral.value);
		if (expr is FloatLiteralExprAst floatLiteral)
			return LLVMValueRef.CreateConstReal(LLVMTypeRef.Float, floatLiteral.value);
		if (expr is BiExprAst biExpr)
			return builder.BuildBinOp((LLVMOpcode) biExpr.op, builder.Build(biExpr.left), builder.Build(biExpr.right));
		return default;
	}
}