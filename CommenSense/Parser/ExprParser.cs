using System.Security.AccessControl;

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
			left = new BiExprAst(BiOpcode(op.kind), op, left, right);
		}

		return left;
	}

	ExprAst PreExpr()
	{
		if (preOps.Contains(current.kind))
		{
			Token token = Next();
			Enum op = PreOpcode(token.kind);
			ExprAst operand = PreExpr();
			return new PreExprAst(op, token, operand);
		}

		return PostExpr(PrimExpr());
	}

	ExprAst PostExpr(ExprAst operand)
	{
	Loop:
		if (postOps.Contains(current.kind))
		{
			Token token = Next();
			Enum op = PostOpcode(token.kind);
			operand = new PostExprAst(token, op, operand);
			goto Loop;
		}
		if (current.kind is TokenKind.LeftParen)
		{
			Token open = Next();

			List<ExprAst> args = new List<ExprAst>();
			while (current.kind is not TokenKind.Eof and not TokenKind.RightParen)
			{
				args.Add(Expr());
				if (current.kind is not TokenKind.RightParen)
					Match(TokenKind.Comma);
			}

			Match(TokenKind.RightParen);
			operand = new CallExprAst(operand, open, args.ToArray());
			goto Loop;
		}
		if (current.kind is TokenKind.Dot)
		{
			Next();
			Token name = Match(TokenKind.Identifier);
			operand = new MemberExprAst(operand, name);
			goto Loop;
		}
		if (current.kind is TokenKind.AsKeyword)
		{
			Token keywrd = Next();
			TypeAst to = Type();
			operand = new BitCastExprAst(operand, keywrd, to);
			goto Loop;
		}
		if (current.kind is TokenKind.ToKeyword)
		{
			Token keywrd = Next();
			TypeAst to = Type();
			operand = new CastExprAst(operand, keywrd, to);
			goto Loop;
		}
		if (current.kind is TokenKind.LeftBracket)
		{
			Token open = Next();
			ExprAst idx = Expr();
			Match(TokenKind.RightBracket);
			operand = new IndexExprAst(operand, open, idx);
			goto Loop;
		}

		return operand;
	}

	ExprAst PrimExpr()
	{
		switch (current.kind)
		{
		case TokenKind.Percent:
			Token op = Next();
			return new PreExprAst(PreOpcode(op.kind), op, PrimExpr());
		case TokenKind.Identifier:
		{
			if (scope.FuncExists(current.text)) // fnptr overloads
				return new FuncPtrAst(Next());
			if (scope.VarExists(current.text))
				return new VarExprAst(Next());
			if (!IsType(current))
				BadCode.Report(new SyntaxError($"symbol '{current.text}' doesn't exist", current));
			TypeAst type = Type();
			if (current.kind is TokenKind.LeftBracket)
				return ArrayExpr(eleType: type);
			return GroupExpr(groupType: type);
		}

		case TokenKind.LeftParen:
			Next();
			if (IsType(current))
			{
				TypeAst to = Type();
				Token open = Match(TokenKind.RightParen);
				ExprAst value = PreExpr();
				return new CastExprAst(value, open, to);
			}
			ExprAst expr = Expr();
			Match(TokenKind.RightParen);
			return expr;
		case TokenKind.LeftBracket:
			if (IsType(next))
			{
				Next();
				TypeAst to = Type();
				Token open = Match(TokenKind.RightBracket);
				ExprAst value = PreExpr();
				return new BitCastExprAst(value, open, to);
			}
			return ArrayExpr();

		default:
			return LiteralExpr();
		}
	}

	ArrayExprAst ArrayExpr(TypeAst? eleType = null)
	{
		Token open = Match(TokenKind.LeftBracket);

		List<ExprAst> elements = new List<ExprAst>();
		while (current.kind is not TokenKind.Eof and not TokenKind.RightBracket)
		{
			elements.Add(Expr());

			if (current.kind is not TokenKind.RightBracket)
				Match(TokenKind.Comma);
		}

		Match(TokenKind.RightBracket);
		return new ArrayExprAst(eleType, open, elements.ToArray());
	}

	GroupExprAst GroupExpr(TypeAst groupType)
	{
		Token open = Match(TokenKind.LeftBrace);

		List<ExprAst> members = new List<ExprAst>();
		while (current.kind is not TokenKind.Eof and not TokenKind.RightBrace)
		{
			members.Add(Expr());

			if (current.kind is not TokenKind.RightBrace)
				Match(TokenKind.Comma);
		}
		Match(TokenKind.RightBrace);
		return new GroupExprAst(groupType, open, members.ToArray());
	}

	LiteralExprAst LiteralExpr()
	{
		switch (current.kind)
		{
		case TokenKind.Str:
			return new StrLiteralExprAst(current, Next().text);
		case TokenKind.Char:
			return new CharLiteralExprAst(current, Next().text[0], 8);
		case TokenKind.NullKeyword:
			return new NullLiteralExprAst(Next());
		case TokenKind.Int:
			return new IntLiteralExprAst(current, ulong.Parse(Next().text));
		case TokenKind.Float:
			return new FloatLiteralExprAst(current, double.Parse(Next().text));
		case TokenKind.TrueKeyword:
			Next();
			return new BoolLiteralExprAst(current, true);
		case TokenKind.FalseKeyword:
			Next();
			return new BoolLiteralExprAst(current, false);

		default:
			BadCode.Report(new SyntaxError("not a literal", current));
			return new LiteralExprAst(current);
		}
	}
}