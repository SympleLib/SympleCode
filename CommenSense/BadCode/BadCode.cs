namespace CommenSense;

public static class BadCode
{
	public static List<SyntaxError> errors { get; private set; } = new List<SyntaxError>();
	//readonly List<SyntaxWarning> warnings;

	internal static void Report(SyntaxError error) =>
		errors.Add(error);

	public static void Dump()
	{
		foreach (SyntaxError error in errors)
			Console.WriteLine(error);

		errors = new List<SyntaxError>();
	}
}