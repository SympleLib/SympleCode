namespace CommenSense;

partial class Parser
{
	readonly Lexer lxr;
	Token current = new Token(TokenKind.Unknown, string.Empty);

	public Parser(string source) =>
		lxr = new Lexer(source);

	public Ast Parse()
	{
		Next();
		return BiExpr();
	}

	Token Next()
	{
		Token tmp = current;
		current = lxr.LexNext();
		return tmp;
	}
}