namespace CommenSense;

partial class Parser
{
	Scope scope;

	void EnterScope() =>
		scope = new Scope(this, scope);

	void ExitScope() =>
		scope = scope.parent!;

	class Scope
	{
		public readonly Parser parser;
		public readonly Scope? parent;
		readonly List<string> vars = new List<string>();
		readonly List<string> funcs = new List<string>();

		public Scope(Parser parser, Scope? parent = null)
		{
			this.parser = parser;
			this.parent = parent;
		}

		public bool VarExists(string name)
		{
			if (vars.Contains(name))
				return true;
			if (parent is not null)
				return parent!.VarExists(name);
			return parser.varNames.ContainsKey(name);
		}

		public bool FuncExists(string name)
		{
			if (funcs.Contains(name))
				return true;
			if (parent is not null)
				return parent!.FuncExists(name);
			return parser.funcNames.ContainsKey(name);
		}

		public void DefineVar(string name) =>
			vars.Add(name);

		public void DefineFunc(string name) =>
			funcs.Add(name);
	}
}