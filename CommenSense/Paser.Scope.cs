﻿namespace CommenSense;

partial class Parser
{
	Scope scope = new Scope();

	void EnterScope() =>
		scope = new Scope(scope);

	void ExitScope() =>
		scope = scope.parent!;

	class Scope
	{
		public readonly Scope? parent;
		readonly List<string> vars = new List<string>();

		public Scope(Scope? parent = null) =>
			this.parent = parent;

		public bool VarExists(string name)
		{
			if (vars.Contains(name))
				return true;
			if (parent is not null)
				return parent!.VarExists(name);
			return false;
		}

		public void DefineVar(string name) =>
			vars.Add(name);
	}
}