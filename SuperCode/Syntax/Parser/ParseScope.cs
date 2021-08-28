namespace SuperCode
{
	public class ParseScope
	{
		public ParseScope? parent;
		public readonly List<string> vars = new List<string>();

		public ParseScope(ParseScope? parent = null) =>
			this.parent = parent;
	}
}
