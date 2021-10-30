namespace CommenSense;

partial class Parser
{
	static readonly List<string> varNames = new List<string>();
	static readonly List<string> funcNames = new List<string>();
	static readonly List<string> typeNames = new List<string>()
	{
		"void",
		"bool",
		"char",
		"int",
		"long",
		"float",
		"double",
	};

	public static readonly Dictionary<string, Parser> parsers = new Dictionary<string, Parser>();
}