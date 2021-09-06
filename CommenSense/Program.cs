global using LLVMSharp.Interop;

using static LLVMSharp.Interop.LLVMOpcode;

LLVMValueRef Build(LLVMBuilderRef builder, ExprAst expr)
{
	if (expr is IntLiteralExprAst intLiteral)
		return LLVMValueRef.CreateConstInt(LLVMTypeRef.Int32, intLiteral.value);
	if (expr is BiExprAst biExpr)
		return builder.BuildBinOp(biExpr.op, Build(builder, biExpr.left), Build(builder, biExpr.right));
	return default;
}

const string src = "6 + 9/ 7";

ExprAst expr = new BiExprAst(LLVMAdd, new IntLiteralExprAst(6), new BiExprAst(LLVMSDiv, new IntLiteralExprAst(9), new IntLiteralExprAst(7)));
var builder = LLVMBuilderRef.Create(LLVMContextRef.Global);
var value = Build(builder, expr);
long result = value.ConstIntSExt;
Console.WriteLine(result);
Console.ReadKey();

record Ast;

record ExprAst: Ast;
record IntLiteralExprAst(ulong value): ExprAst;
record BiExprAst(LLVMOpcode op, ExprAst left, ExprAst right): ExprAst;