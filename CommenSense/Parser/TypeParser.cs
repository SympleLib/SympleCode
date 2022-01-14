namespace CommenSense;

partial class Parser
{
	TypeAst TypeExt(TypeAst baseType)
	{
		bool mutable = false;
		if (current.kind is TokenKind.MutableKeyword)
		{
			mutable = true;
			Next();
		}

		switch (current.kind)
		{
		case TokenKind.Star:
			return TypeExt(PtrType(baseType, mutable));
		case TokenKind.LeftBracket:
			return TypeExt(ArrayType(baseType, mutable));
		case TokenKind.LeftParen:
			return TypeExt(FuncType(baseType, mutable));
		default:
			return baseType;
		}
	}

	TypeAst Type()
	{
		Token typeBase = Match(TokenKind.Identifier);
		TypeAst baseType = new BaseTypeAst(typeBase);

		return TypeExt(baseType);
	}

	TypeAst PtrType(TypeAst baseType, bool mutable)
	{
		Token token = Match(TokenKind.Star);
		return new PtrTypeAst(baseType, mutable, token);
	}

	TypeAst ArrayType(TypeAst baseType, bool mutable)
	{
		Token open = Match(TokenKind.LeftBracket);
		ExprAst length = Expr();
		Match(TokenKind.RightBracket);

		return new ArrayTypeAst(baseType, mutable, open, length);
	}

	TypeAst FuncType(TypeAst baseType, bool mutable)
	{
		if (mutable)
			throw new Exception("func code cannot be mutable");
		
		Token open = Match(TokenKind.LeftParen);

		bool vaArg = false;
		List<TypeAst> paramTypes = new List<TypeAst>();
		while (current.kind is not TokenKind.Eof and not TokenKind.RightParen)
		{
			if (current.kind is TokenKind.DotDotDot)
			{
				vaArg = true;
				break;
			}

			paramTypes.Add(Type());

			if (current.kind is not TokenKind.RightParen)
				Match(TokenKind.Comma);
		}

		Match(TokenKind.RightParen);
		
		return new FuncTypeAst(baseType, open, paramTypes.ToArray(), vaArg);
	}
}