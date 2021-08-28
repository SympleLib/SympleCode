namespace SuperCode
{
	public class ParseScope
	{
		public ParseScope? parent;
		public readonly List<string> vars = new List<string>();

		public ParseScope(ParseScope? parent = null)
		{
			if (parent is null)
				return;

			vars = parent.vars;
		}
	}
}
