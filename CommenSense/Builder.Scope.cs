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
		readonly Dictionary<string, Value> symbols = new Dictionary<string, Value>();

		public Scope(Builder builder, Scope? parent = null)
		{
			this.builder = builder;
			this.parent = parent;
		}

		public Value Find(string name)
		{
			if (symbols.TryGetValue(name, out Value symbol))
				return symbol;
			if (parent is not null)
				return parent!.Find(name);
			Value global = builder.llModule.GetNamedGlobal(name);
			if (global == null)
				return builder.llModule.GetNamedFunction(name);
			return global;
		}

		public void Define(string name, Value symbol) =>
			symbols.Add(name, symbol);
	}
}