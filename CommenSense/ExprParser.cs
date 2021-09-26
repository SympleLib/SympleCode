namespace CommenSense;

partial class Parser
{
	static readonly TokenKind[] unOps = {
		TokenKind.Plus,
		TokenKind.Minus,

		TokenKind.And,
	};

	static Enum UnOpcode(TokenKind kind) =>
		kind switch
		{
			TokenKind.Plus => LLVMAdd,
			TokenKind.Minus => LLVMFNeg,
			TokenKind.And => LLVMAnd,
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

		return PostExpr(PrimExpr());
	}

	ExprAst PostExpr(ExprAst operand)
	{
	Loop:
		if (current.kind is TokenKind.LeftParen)
		{
			Next();

			List<ExprAst> args = new List<ExprAst>();
			while (current.kind is not TokenKind.Eof and not TokenKind.RightParen)
			{
				args.Add(Expr());
				if (current.kind is not TokenKind.RightParen)
					Match(TokenKind.Comma);
			}

			Match(TokenKind.RightParen);
			operand = new CallExprAst(operand, args.ToArray());
			goto Loop;
		}

		return operand;
	}

	ExprAst PrimExpr()
	{
		switch (current.kind)
		{
		case TokenKind.Identifier:
			if (scope.FuncExists(current.text) || next.kind is TokenKind.LeftParen)
				return new FuncPtrAst(Next().text);
			return new VarExprAst(Next().text);

		default:
			return LiteralExpr();
		}
	}

	LiteralExprAst LiteralExpr()
	{
		switch (current.kind)
		{
		case TokenKind.Str:
			return new StrLiteralExprAst(Next().text);
		case TokenKind.Char:
			return new CharLiteralExprAst((ulong) Next().text[0], 8);
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