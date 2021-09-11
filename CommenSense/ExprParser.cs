namespace CommenSense;

partial class Parser
{
	static int BiPrecendence(TokenKind kind) =>
		   kind switch
		   {
			   TokenKind.Star or TokenKind.Slash or TokenKind.Percent => 2,
			   TokenKind.Plus or TokenKind.Minus => 1,
			   _ => 0,
		   };

	static Enum BiOpcode(TokenKind kind) =>
		kind switch
		{
			TokenKind.Plus => LLVMAdd,
			TokenKind.Minus => LLVMSub,
			TokenKind.Star => LLVMMul,
			TokenKind.Slash => LLVMSDiv,
			TokenKind.Percent => LLVMSRem,
			_ => (LLVMOpcode) 0,
		};

	ExprAst Expr() => BiExpr();

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
		case TokenKind.Float:
			return new FloatLiteralExprAst(double.Parse(Next().text));

		default:
			throw new Exception("not a literal");
		}
	}
}