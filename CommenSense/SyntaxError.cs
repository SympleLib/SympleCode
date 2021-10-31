namespace CommenSense;

public class SyntaxError: Exception
{
	public override string Message { get; }

	internal SyntaxError(string msg, Parser.Token token) =>
		Message = $"line {token.line}, col {token.col}: {msg}";
}