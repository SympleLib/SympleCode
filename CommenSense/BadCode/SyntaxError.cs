namespace CommenSense;

public record class SyntaxError(string msg, Token token)
{
	public override string ToString() =>
		$"line {token.line}, col {token.col}: {msg}";
}
