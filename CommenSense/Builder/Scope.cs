namespace CommenSense;

using Value = LLVMValueRef;

partial class Builder
{
	Scope scope;

	void EnterScope() =>
		scope = new Scope(this, scope);

	void ExitScope() =>
		scope = scope.parent!;

	class Scope
	{
		public readonly Builder builder;
		public readonly Scope? parent;
		readonly Dictionary<string, Container> ctnrs = new Dictionary<string, Container>();
		readonly Dictionary<string, Value> symbols = new Dictionary<string, Value>();

		public Scope(Builder builder, Scope? parent = null)
		{
			this.builder = builder;
			this.parent = parent;
		}

		public void Define(string name, Container ctnr) =>
			ctnrs.Add(name, ctnr);

		public Container GetCtnr(string name)
		{
			if (ctnrs.TryGetValue(name, out Container? ctnr))
				return ctnr;
			if (parent is not null)
				return parent!.GetCtnr(name);
			throw new Exception("ctnr don't exist man");
		}

		public Value Find(string name)
		{
			if (symbols.TryGetValue(name, out Value symbol))
				return symbol;
			if (parent is not null)
				return parent!.Find(name);
			throw new Exception("symbol don't exist man");
		}

		public void Define(string name, Value symbol) =>
			symbols.Add(name, symbol);
	}
}