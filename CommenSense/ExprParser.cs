namespace CommenSense;

partial class Parser
{
	static readonly TokenKind[] unOps = {
		TokenKind.Plus,
		TokenKind.Minus,
	};

	static Enum UnOpcode(TokenKind kind) =>
		kind switch
		{
			TokenKind.Plus => LLVMAdd,
			TokenKind.Minus => LLVMFNeg,
			_ => (LLVMOpcode) 0,
		};

	static int BiPrecendence(TokenKind kind) =>
		   kind switch
		   {
			   TokenKind.Star or TokenKind.Slash or TokenKind.Percent or TokenKind.SlashDash or TokenKind.PercentDash => 2,
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
			TokenKind.SlashDash => LLVMUDiv,
			TokenKind.PercentDash => LLVMURem,
			_ => (LLVMOpcode) 0,
		};

	ExprAst Expr() => BiExpr();

	ExprAst BiExpr(int parentPrecedence = 0)
	{
		ExprAst left = PreExpr();
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

	ExprAst PreExpr()
	{
		if (unOps.Contains(current.kind))
		{
			Enum op = UnOpcode(Next().kind);
			ExprAst operand = PreExpr();
			return new UnExprAst(op, operand);
		}

		return PrimExpr();
	}

	ExprAst PrimExpr()
	{
		switch (current.kind)
		{
		case TokenKind.Identifier:
			return new VarExprAst(Next().text);

		default:
			return LiteralExpr();
		}
	}

	LiteralExprAst LiteralExpr()
	{
		switch (current.kind)
		{
		case TokenKind.Int:
			return new IntLiteralExprAst(ulong.Parse(Next().text));
		case TokenKind.Float:
			return new FloatLiteralExprAst(double.Parse(Next().text));
		case TokenKind.TrueKeyword:
			Next();
			return new BoolLiteralExprAst(true);
		case TokenKind.FalseKeyword:
			Next();
			return new BoolLiteralExprAst(false);

		default:
			throw new Exception("not a literal");
		}
	}
}