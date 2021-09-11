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

const string src = "6 + 9 / 9";

Parser parser = new Parser(src);
Ast ast = parser.Parse();
Console.WriteLine(ast);

using LLVMBuilderRef builder = LLVMBuilderRef.Create(LLVMContextRef.Global);
LLVMValueRef val = Build(builder, (ExprAst) ast);
Console.WriteLine(val.ConstIntSExt);
Console.ReadKey();

class Parser
{
	enum TokenKind
	{
		Unknown = -1,
		Eof,

		Int,
		Float,
		Identifier,

		Plus,
		Minus,
		Star,
		Slash,
		Percent,
	}

	record Token(TokenKind kind, string text);

	static int BiPrecendence(TokenKind kind) =>
		kind switch
		{
			TokenKind.Star or TokenKind.Slash or TokenKind.Percent => 2,
			TokenKind.Plus or TokenKind.Minus => 1,
			_ => 0,
		};

	static LLVMOpcode BiOpcode(TokenKind kind) =>
		kind switch
		{
			TokenKind.Plus => LLVMAdd,
			TokenKind.Minus => LLVMSub,
			TokenKind.Star => LLVMMul,
			TokenKind.Slash => LLVMSDiv,
			TokenKind.Percent => LLVMSRem,
			_ => (LLVMOpcode) 0,
		};

	readonly Lexer lxr;
	Token current = new Token(TokenKind.Unknown, string.Empty);

	public Parser(string source) =>
		lxr = new Lexer(source);

	public Ast Parse()
	{
		Next();
		return BiExpr();
	}

	ExprAst BiExpr(int parentPrecedence = 0)
	{
		ExprAst left = LiteralExpr();
		while (true)
		{
			int precedence = BiPrecendence(current.kind);
			if (precedence is 0 || precedence < parentPrecedence)
				break;

			Token op = Next();
			ExprAst right = BiExpr(precedence);
			left = new BiExprAst(BiOpcode(op.kind), left, right);
		}

		return left;
	}

	LiteralExprAst LiteralExpr()
	{
		switch (current.kind)
		{
		case TokenKind.Int:
			return new IntLiteralExprAst(ulong.Parse(Next().text));

		default:
			throw new Exception("not a literal");
		}
	}

	class Lexer
	{
		const TokenKind punctuatorStart = TokenKind.Plus;
		static readonly string[] punctuators = {
			"+",
			"-",
			"*",
			"/",
			"%",
		};

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

			if (char.IsDigit(current))
				return Num();
			if (char.IsLetter(current))
				return Identifier();
			return Punctuator();
		}

		Token Num()
		{
			int start = pos;
			TokenKind kind = TokenKind.Int;
			while (char.IsDigit(current) || current is '.')
			{
				if (current is '.')
				{
					if (kind == TokenKind.Int)
						kind = TokenKind.Float;
					else
						throw new Exception("too many dots");
				}

				pos++;
			}

			return new Token(kind, src[start..pos]);
		}

		Token Identifier()
		{
			int start = pos;
			while (char.IsLetterOrDigit(current))
				pos++;

			return new Token(TokenKind.Identifier, src[start..pos]);
		}

		Token Punctuator()
		{
			for (int i = punctuators.Length - 1; i >= 0; i--)
				if (src[pos..].StartsWith(punctuators[i]))
					return new Token(punctuatorStart + i, src[pos..(pos += punctuators[i].Length)]);

			return new Token(TokenKind.Unknown, src[pos..++pos]);
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

	Token Next()
	{
		Token tmp = current;
		current = lxr.LexNext();
		return tmp;
	}
}

record Ast;

record ExprAst: Ast;
record LiteralExprAst(): ExprAst;
record IntLiteralExprAst(ulong value): LiteralExprAst;
record BiExprAst(LLVMOpcode op, ExprAst left, ExprAst right): ExprAst;