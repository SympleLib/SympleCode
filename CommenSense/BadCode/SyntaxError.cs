namespace CommenSense;

public record SyntaxError(string msg, Token token)
{
	public override string ToString() =>
		$"line {token.line}, col {token.col}: {msg}";
}
