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

Parser parser = new Parser();
parser.Parse();
Console.ReadKey();

class Parser
{
	enum TokenKind
	{
		Unknown = -1,
		Eof,

		Identifier,
	}

	record Token(TokenKind kind, string text);

	public void Parse()
	{
		Lexer lxr = new Lexer("this is a T3ST");

		while (true)
		{
			Token tok = lxr.LexNext();
			Console.WriteLine(tok);
			if (tok.kind is TokenKind.Eof)
				break;
		}
	}

	class Lexer
	{
		readonly string src;
		int pos;
		char current => Peek();

		public Lexer(string source) =>
			src = source;

		public Token LexNext()
		{
			while (char.IsWhiteSpace(current))
				pos++;

			if (current is '\0')
				return new Token(TokenKind.Eof, string.Empty);

			if (char.IsLetter(current))
				return Identifier();

			return new Token(TokenKind.Unknown, src[pos..pos]);
		}

		Token Identifier()
		{
			int start = pos;
			while (char.IsLetterOrDigit(current))
				pos++;

			return new Token(TokenKind.Identifier, src[start..pos]);
		}

		char Next()
		{
			char c = current;
			if (pos < src.Length)
				pos++;
			return c;
		}

		char Peek(int offset = 0)
		{
			int i = pos + offset;
			if (i < src.Length)
				return src[i];
			return '\0';
		}
	}

}

record Ast;

record ExprAst: Ast;
record IntLiteralExprAst(ulong value): ExprAst;
record BiExprAst(LLVMOpcode op, ExprAst left, ExprAst right): ExprAst;