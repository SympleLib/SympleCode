namespace CommenSense;

partial class Parser
{
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
		else if (current.kind is TokenKind.Dot)
		{
			Next();
			string memberName = Name();
			operand = new MemberExprAst(operand, memberName);
			goto Loop;
		}
		else if (current.kind is TokenKind.AsKeyword)
		{
			Next();
			TypeAst to = Type();
			operand = new BitCastExprAst(operand, to);
			goto Loop;
		}
		else if (current.kind is TokenKind.ToKeyword)
		{
			Next();
			TypeAst to = Type();
			operand = new CastExprAst(operand, to);
			goto Loop;
		}
		else if (current.kind is TokenKind.LeftBracket)
		{
			Next();
			ExprAst idx = Expr();
			Match(TokenKind.RightBracket);
			operand = new IndexExprAst(operand, idx);
			goto Loop;
		}

		return operand;
	}

	ExprAst PrimExpr()
	{
		switch (current.kind)
		{
		case TokenKind.Percent:
			return new UnExprAst(UnOpcode(Next().kind), PrimExpr());
		case TokenKind.Identifier:
			if (IsType(current))
				return ArrayExpr(eleType: Type());

			if (scope.FuncExists(current.text) || next.kind is TokenKind.LeftParen)
				return new FuncPtrAst(Next().text);
			return new VarExprAst(Next().text);
		case TokenKind.LeftParen:
			Next();
			if (IsType(current))
			{
				TypeAst to = Type();
				Match(TokenKind.RightParen);
				ExprAst value = PreExpr();
				return new CastExprAst(value, to);
			}
			ExprAst expr = Expr();
			Match(TokenKind.RightParen);
			return expr;
		case TokenKind.LeftBracket:
			if (IsType(next))
			{
				Next();
				TypeAst to = Type();
				Match(TokenKind.RightBracket);
				ExprAst value = PreExpr();
				return new BitCastExprAst(value, to);
			}
			return ArrayExpr();

		default:
			return LiteralExpr();
		}
	}

	ArrayExprAst ArrayExpr(TypeAst? eleType = null)
	{
		Match(TokenKind.LeftBracket);

		List<ExprAst> elements = new List<ExprAst>();
		while (current.kind is not TokenKind.Eof and not TokenKind.RightBracket)
		{
			elements.Add(Expr());

			if (current.kind is not TokenKind.RightBracket)
				Match(TokenKind.Comma);
		}

		Match(TokenKind.RightBracket);
		return new ArrayExprAst(eleType, elements.ToArray());
	}

	LiteralExprAst LiteralExpr()
	{
		switch (current.kind)
		{
		case TokenKind.Str:
			return new StrLiteralExprAst(Next().text);
		case TokenKind.Char:
			return new CharLiteralExprAst(Next().text[0], 8);
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